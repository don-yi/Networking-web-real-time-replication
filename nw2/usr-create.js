// require
const util = require('./util.js');
const usrman = require('./usr-manager.js');

module.exports = (req, res) => {
	// gen id deterministically w/ usrname
	const id = util.GenId(req.body.username);

	// if dup, err status
	if (usrman.UsrFound(id)) {
		res.sendStatus(409);
		return;
	};

	// create new usr
	var newUsr = usrman.CreateUsr(id, req.body);
	// res w/ json & success status
	res.status(200).json(newUsr);
};
