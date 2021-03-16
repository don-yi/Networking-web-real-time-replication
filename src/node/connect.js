// require
const crypto = require('crypto');
const util = require('./util.js');

module.exports = (app, usrCollection, redisCli) => {
	app.post(util.PATH + 'connect', (req, res) => {
    var gamType = req.body.game_type;
    // err : no gam type
    if (!gamType) { res.sendStatus(400); return; }

    // err : no session
    var session = req.body.session;
    if (!session) { res.sendStatus(401); return; }

    var sessionKey = `sessions:${session}`;
    redisCli.get(sessionKey, (err, uid) => {
      // err : bad/old session
      if (!uid) { res.sendStatus(401); return; };

      // find doc by query
      var query = { id : uid };
      usrCollection.findOne(query, (err, usrObj) => {

        var username = usrObj.username;
        var avatar = usrObj.avatar;
        var plaintextToken = username + avatar + gamType + util.SECRET;
        var token
          = crypto.createHash('sha256').update(plaintextToken).digest('base64');

        res.status(200).json({
          username:   username,
          avatar:     avatar,
          game_port:  util.GAM_PORT,
          token:      token
        })
      });
    });
  });
};
