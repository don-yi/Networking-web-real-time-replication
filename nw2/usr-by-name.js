// import
const util = require('./util.js');

module.exports = (app, usrCollection, redisCli) => {
	app.get(util.PATH + 'users', (req, res) => {
    var unameToRetrieve = req.query.username;
    // err w/ no uname
    if (!unameToRetrieve) { res.sendStatus(400); return; }

    var session = req.body.session;

    // err w/ no session
    if (!session) { res.sendStatus(401); return; }

    var sessionKey = `sessions:${session}`;
    redisCli.get(sessionKey, (err, retrieverId) => {
      // err w/ bad session
      if (!retrieverId) { res.sendStatus(401); return; }

      queryWithUname = { username : unameToRetrieve };
      usrCollection.findOne(queryWithUname, (err, usrObj) => {
        // err w/ bad uname
        if (!usrObj) { res.sendStatus(404); return; };

        // verify owner
        var idToRetrieve = util.GenId(unameToRetrieve);
        var usrObjToReturn = retrieverId === idToRetrieve ? usrObj : {
          id:       usrObj.id,
          username: usrObj.username,
          avatar:   usrObj.avatar
        };
        // res w/ json & success status
        res.status(200).json(usrObjToReturn);
      });
    });
  });
};
