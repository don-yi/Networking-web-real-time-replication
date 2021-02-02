// require
const base64url = require('base64url');
const myapp = require('./app.js');

module.exports = (req, res) => {
	// Create User
	// Path: /api/v1/users/
	// Verb: POST
	// Authenticated: No
	// Request Body:
	// •	username
	// •	password
	// •	avatar
	// Response Body:
	// •	id (the user ID)
	// •	username
	// •	password
	// •	avatar
	// Creates a new user, succeeding if that user did not already exist.

	// gen id deterministically w/ usrname
  const reqId = base64url(req.body.username);

  var usrmap = myapp.usrmap;

	// err status when dupl usr
	if (usrmap.has(reqId)) {
		res.status(409);
		return;
	};

	// set new usr to map
	usrmap.set(reqId, {
		username: req.body.username,
		password: req.body.password,
		avatar: req.body.avatar
	});

  // created usr json for res
	const createdUsr = {
		id: reqId,
		username: usrmap.get(reqId).username,
		password: usrmap.get(reqId).password,
		avatar: usrmap.get(reqId).avatar
	};

	// res w/ json & success status
	res.json(createdUsr);
	res.status(200);
};
