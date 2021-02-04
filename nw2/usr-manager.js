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
  CreateUsr = (id, json) => {
    const newUsr = {
      id: id,
      username: json.username,
      password: json.password,
      avatar:   json.avatar
    };
    this.allUsr.push(newUsr);
    return newUsr;
  };

  // check pw match
  IsBadPw = (json) => {
    return this.allUsr.find((usr) => {
      if (usr.username === json.username)
        return (usr.password != json.password) ? true : false;
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

  // retrieve usr obj from container depends on whether owner / non owner
  RetrieveUsr = (id, session) => {
    // get matched session
    var matchedSession = this.allSession.find((elem) => {
      if (elem.session === session) return elem.session;
    });

    // get usr from usr container
    var usr = this.allUsr.find((usr) => { if (usr.id === id) return usr; });

    // verify owner and return
    return matchedSession.id === id ? usr : {
      id:       usr.id,
      username: usr.username,
      avatar:   usr.avatar
    };
  };
};

const usrman = new UserManager();
module.exports = {
  UsrFound:       usrman.UsrFound,
  CreateUsr:      usrman.CreateUsr,
  IsBadPw:        usrman.IsBadPw,
  CreateSession:  usrman.CreateSession,
  RetrieveUsr:    usrman.RetrieveUsr
};
