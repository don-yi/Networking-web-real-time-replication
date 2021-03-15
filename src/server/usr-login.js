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
			var sessionKey = `sessions:${session}`;
			var uid = util.GenId(uname);
			redisCli.set(sessionKey, uid, (err, reply) => {
				// session expire after 10s
				// redisCli.expire(sessionKey, 10);

				// successful Login should remove any previous sessions for that user
				// get old session from lookup & del
				var lookupKey = `sessionsIdsByUserId:${uid}`;
				redisCli.get(lookupKey, (err, reply) => {
					if (reply) redisCli.del(reply);
				});
				redisCli.set(lookupKey, sessionKey, (err, reply) => {
					// lookup expire after 10s
					// redisCli.expire(lookupKey, 10);
					// res w/ success status & session
					res.status(200).json({ session : session });
				});
			});
		});
	});
};
