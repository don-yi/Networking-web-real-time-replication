// require
const util = require('./util.js');
const usrman = require('./usr-manager.js');

module.exports = (req, res) => {
  // Response Body:
  // •	session (the ID of the session)
  // Creates a session for the user provided,
  // if the user exists and the password matches.
  // Only one session should be active per user.

	// get deterministic id w/ usrname
	const id = util.GenId(req.body.username);

	// err status: bad usrname
	if (!usrman.UsrFound(id)) {
		res.sendStatus(400);
		return;
  };
  // err status: bad pw
  if (usrman.IsBadPw(req.body)) {
		res.sendStatus(403);
		return;
  };

  // gen new session
  const session = util.GenSession();

	// res w/ success status & session
	res.status(200).json({ session: session });
};
