// require
const usrman = require('./usr-manager.js');
const util = require('./util.js');

module.exports = (app, usrCollection, redisCli) => {
	app.put(util.PATH + 'users/:id', (req, res) => {
    var session = req.body.session;

    // err w/ no session
    if (!session) { res.sendStatus(401); return; }

    var sessionKey = `sessions:${session}`;
    redisCli.get(sessionKey, (err, updaterId) => {
      // err w/ bad session
      if (!updaterId) { res.sendStatus(401); return; }

      var idToUpdate = req.params.id;
      var queryWithId = { id : idToUpdate };
      usrCollection.findOne(queryWithId, (err, usrObj) => {
        // err w/ bad id
        if (!usrObj) { res.sendStatus(404); return; };

        // update fail on diff usr
        if (updaterId !== idToUpdate) { res.sendStatus(403); return; }

        // update w/ new info
        var newInfo = req.body;
        var setCommand = { $set: {
          username: newInfo.username,
          password: newInfo.password,
          avatar:   newInfo.avatar
        } };
        var setOptions = { returnOriginal: false };
        usrCollection.findOneAndUpdate(
          queryWithId, setCommand, setOptions, (err, result) => {
          // res w/ json & success status
          res.status(200).json(result.value);
        });
      });
    });
  });
};
