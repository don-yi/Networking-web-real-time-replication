// require
const base64url = require('base64url');
const uuid4 = require('uuid4');
const usrmap = require('./app.js');

module.exports = (req, res) => {
  // Path: /api/v1/login
  // Verb: POST
  // Authenticated: No
  // Request Body: 
  // •	username
  // •	password
  // Response Body:
  // •	session (the ID of the session)
  // Creates a session for the user provided,
  // if the user exists and the password matches.
  // Only one session should be active per user.

	// get deterministic id w/ usrname
  const id = base64url(req.body.username);

	// err status: bad usrname
	if (!usrmap.has(id)) {
		res.status(400).json();
		return;
  };
  // err status: bad pw
  if (req.body.password != usrmap.get(id).password) {
		res.status(403).json();
		return;
  };

  console.log(req.body);

  // Generate a new UUID
  var session = uuid4();

  // login info json for res
	const loginInfo = {
		session: session,
	};

	// res w/ login info json & success status
	res.json(loginInfo);
	// res w/ success status
	res.status(200);
};
