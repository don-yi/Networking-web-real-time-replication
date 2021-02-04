// require
const usrman = require('./usr-manager.js');
const util = require('./util.js');

module.exports = (req, res) => {
  // get session from req, uname from query, id from uname,
  // and matching session-id
  var uname = req.query.username;
  // err w/ no uname
  if (!uname) { res.sendStatus(400); return; }

  var session = req.body.session;
  var id = util.GenId(uname);
  var matchingSessionId = usrman.GetMatchingSessionId(session);

  // err w/ no/bad session
  if (!session || !matchingSessionId) { res.sendStatus(401); return; }
  // err w/ bad uname
  if (!usrman.UsrFound(id)) { res.sendStatus(404); return; };

	// retrieve usr info
	var retrievedUsr = usrman.RetrieveUsr(id, session);
	// res w/ json & success status
  res.status(200).json(retrievedUsr);
};
