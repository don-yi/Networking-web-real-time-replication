// require
const usrman = require('./usr-manager.js');
const util = require('./util.js');

module.exports = (req, res) => {
  var id = req.params.id;
  var session = req.body.session;
  var matchingSessionId = usrman.GetMatchingSessionId(session);

  // err w/ no/bad session
  if (!session || !matchingSessionId) { res.sendStatus(401); return; }
  // err w/ bad uname
  if (!usrman.IsUsrFound(id)) { res.sendStatus(404); return; };

	// update usr info
  var updatedUsr = usrman.UpdateUsr(req.body, session, id);
  // update fail on diff usr
  if (!updatedUsr) { res.sendStatus(403); return; }
	// res w/ json & success status
  res.status(200).json(updatedUsr);
};
