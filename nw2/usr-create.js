// require
const util = require('./util.js');
const usrman = require('./usr-manager.js');

module.exports = (app, usrCollection) => {
	app.post(util.PATH + 'users', (req, res) => {
		var uname = req.body.username;

		// find doc by query
    var query = { username : uname };
		usrCollection.findOne(query, (err, result) => {
			// if dup, err status
			if (result != undefined) {
				res.sendStatus(409); return;
			}
		});

		// create & ins new usr
		var newUsr = usrman.CreateNewUsrObj(util.GenId(uname), req.body);
		usrCollection.insertOne(newUsr, (err, result) => {
			if (err) throw err;
			// res w/ json & success status
			res.status(200).json(newUsr); return;
		});
	});
};
