// require
const util = require('./util.js');
const usrman = require('./usr-manager.js');

module.exports = (req, res) => {
	// get deterministic id w/ usrname
	const id = util.GenId(req.body.username);

	// err status: bad usrname
	if (!usrman.IsUsrFound(id)) { res.sendStatus(400); return; };
  // err status: bad pw
  if (usrman.IsBadPw(req.body)) { res.sendStatus(403); return; };
	// res w/ success status & session
	res.status(200).json(usrman.CreateSession(id));
};
