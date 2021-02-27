// import
const util = require('./util.js');

module.exports = (app, usrCollection, redisCli) => {
	app.get(util.PATH + 'users/:id', (req, res) => {
    var session = req.body.session;

    // err : no session
    if (!session) { res.sendStatus(401); return; };

    var sessionKey = `sessions:${session}`;
    redisCli.get(sessionKey, (err, retrieverId) => {
      // err : bad/old session
      if (!retrieverId) { res.sendStatus(401); return; };

      // find by id
      idToRetrieve = req.params.id;
      var queryWithId = { id : idToRetrieve };
      usrCollection.findOne(queryWithId, (err, usrObjFound) => {
        // err w/ bad id
        if (!usrObjFound) { res.sendStatus(404); return; }

        // verify owner and 
        usrObjToReturn = retrieverId === idToRetrieve ? usrObjFound : {
          id:       usrObjFound.id,
          username: usrObjFound.username,
          avatar:   usrObjFound.avatar
        }
        // res w/ json & success status
        res.status(200).json(usrObjToReturn);
      });
    });
  });
};
