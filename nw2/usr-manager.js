const util = require('./util.js');

class UserManager {
  // container
  allUsr = [];
  allSession = [];

  // find usr in container
  UsrFound = (id) => {
    return this.allUsr.find((usr) => {
      return (usr.id === id) ? true : false;
    });
  };

  // gen new usr obj and push to container
  CreateUsr = (id, uinfo) => {
    const newUsr = {
      id: id,
      username: uinfo.username,
      password: uinfo.password,
      avatar:   uinfo.avatar
    };
    this.allUsr.push(newUsr);
    return newUsr;
  };

  // check pw match
  IsBadPw = (uinfo) => {
    return this.allUsr.find((usr) => {
      if (usr.username === uinfo.username)
        return (usr.password != uinfo.password) ? true : false;
    });
  };

  // create new session
  CreateSession = (id) => {
    const sessionObj = {
      session:  util.GenSession(),
      id:       id
    };
    this.allSession.push(sessionObj);
    return sessionObj;
  };

  // get matching session
  GetMatchingSessionId = (session) => {
    return this.allSession.find((sessionId) => {
      if (sessionId.session === session) return sessionId;
    });
  };

  // retrieve usr obj from container depends on whether owner / non owner
  RetrieveUsr = (id, session) => {
    // get mathcing session
    var matchingSession = this.GetMatchingSessionId(session);
    // get usr from usr container
    var usr = this.allUsr.find((usr) => { if (usr.id === id) return usr; });

    // verify owner and return
    return matchingSession.id === id ? usr : {
      id:       usr.id,
      username: usr.username,
      avatar:   usr.avatar
    };
  };

  // update usr from container depends on whether owner / non owner
  UpdateUsr = (newInfo, session, id) => {
    // get mathcing session
    var matchingSessionId = this.GetMatchingSessionId(session);
    // get usr from usr container
    var usr = this.allUsr.find((usr) => { if (usr.id === id) return usr; });

    // verify owner and update
    if (matchingSessionId.id === id) {
      // update uinfo
      usr.username = newInfo.username;
      usr.password = newInfo.password;
      usr.avatar = newInfo.avatar;
      return usr;
    };
  };
};

const usrman = new UserManager();
module.exports = {
  UsrFound:             usrman.UsrFound,
  CreateUsr:            usrman.CreateUsr,
  IsBadPw:              usrman.IsBadPw,
  CreateSession:        usrman.CreateSession,
  GetMatchingSessionId: usrman.GetMatchingSessionId,
  RetrieveUsr:          usrman.RetrieveUsr,
  UpdateUsr:            usrman.UpdateUsr
};
