// require
const util = require('./util.js');

module.exports = (app, usrCollection, redisCli) => {
	app.post(util.PATH + 'login', (req, res) => {
    var uname = req.body.username;

		// find doc by query
    var query = { username : uname };
		usrCollection.findOne(query, (err, result) => {
			// err status: bad usrname
			if (!result) { res.sendStatus(400); return; }
			// err status: bad pw
			if (result.password !== req.body.password) {
				 res.sendStatus(403); return; 
			}

			var session = util.GenSession();
			var key = `sessions:${session}`;
			var uid = util.GenId(uname);
			redisCli.hset(key, "id", uid, (err, reply) => {
				// err status
				if (err) { res.sendStatus(400); return; }
				if (req.body.expire) redisCli.expire(key, 10);
				// res w/ success status & session
				res.status(200).json({ session : session });
			});
		});
	});
};
