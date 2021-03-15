// require
const util = require('./util.js');

module.exports = (app, usrCollection) => {
	app.post(util.PATH + 'users', (req, res) => {

		var uname = req.body.username;

		// find doc by query
    var query = { username : uname };
		usrCollection.findOne(query, (err, result) => {

			// if dup, err status
			if (result) { res.sendStatus(409); return; };

			// create & ins new usr
			var newUsr = CreateNewUsrObj(req.body);
			usrCollection.insertOne(newUsr, (err, result) => {
				if (err) throw err;
				// res w/ json & success status
				res.status(200).json(newUsr); return;
			});
		});
	});
};

/**
 * Generate new user object and push to container.
 *
 * @param  {object} uinfo User info.
 * @return  {object} New user object created in container.
 */
function CreateNewUsrObj (uinfo) {
	const newUsr = {
		id: 			util.GenId(uinfo.username),
		username: uinfo.username,
		password: uinfo.password,
		avatar:   uinfo.avatar
	};
	return newUsr;
};