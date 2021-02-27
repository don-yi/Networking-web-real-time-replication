// require
const util = require('./util.js');
const usrman = require('./usr-manager.js');

module.exports = (app, usrCollection, redisCli) => {
	app.get(util.PATH + 'users/:id', (req, res) => {
    var session = req.body.session;

    // err : no session
    if (!session) { res.sendStatus(401); return; };

    var sessionKey = `sessions:${session}`;
    redisCli.get(sessionKey, (err, retrieverId) => {
      // err : bad/old session
      if (!retrieverId) {
        res.sendStatus(401); return; 
      };

      // find by id
      idToRetrieve = req.params.id;
      var query = { id : idToRetrieve };
      usrCollection.findOne(query, (err, usrObj) => {
        // err w/ bad id
        if (!usrObj) {
          res.sendStatus(404); return;          
        }

        // verify owner and res w/ json & success status
        res.status(200).json(retrieverId === idToRetrieve ? usrObj : {
          id:       usrObj.id,
          username: usrObj.username,
          avatar:   usrObj.avatar
        });
      });
    });
  });
};
