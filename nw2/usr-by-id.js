// require
const usrman = require('./usr-manager.js');

module.exports = (req, res) => {
  // Authenticated: Yes
  // Request Body: none
  // Response Body:
  // •	id (the user ID)
  // •	username
  // •	password (only if the user is the same as the owner of the session)
  // •	avatar
  // Retrieves the specified user by ID (the generated value).
  // The password is only provided if the user requested is the same as
  // the owner of the session.
  // The other fields are accessible for any user by any user.

  // err w/ no session
  if (!req.body.session) {
    res.sendStatus(401);
    return;
  }

	// retrieve usr info
	var retrievedUsr = usrman.RetrieveUsr(req.params.id, req.body.session);
	// res w/ json & success status
  res.status(200).json(retrievedUsr);
};
