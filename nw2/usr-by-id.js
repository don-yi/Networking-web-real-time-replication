// require
const usrman = require('./usr-manager.js');

module.exports = (req, res) => {
  // get session req
  var session = req.body.session;
  var id = req.params.id;
  var matchingSessionId = usrman.GetMatchingSessionId(session);

  // err w/ no/bad/old session
  if (!session || !matchingSessionId) { res.sendStatus(401); return; }

	// retrieve usr info
	var retrievedUsr = usrman.RetrieveUsr(id, session);
	// res w/ json & success status
  res.status(200).json(retrievedUsr);
};
