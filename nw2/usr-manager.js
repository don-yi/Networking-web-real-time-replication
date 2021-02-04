const util = require('./util.js');

class UserManager {
  // container
  allUsr = [];
  allSession = [];

  // find usr in container
  UsrFound = (id) => {
    for (const usr of this.allUsr) if (usr.id == id) return true;
    return false;
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
    for (const usr of this.allUsr) {
      if (usr.username == json.username) {
        if (usr.password != json.password) {
          return true;
        };
        return false;
      };
    };
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
    for (const usr of this.allUsr) if (usr.id == id) {
      var isOwner = this.allSession.find((elem) => {
        if (elem.session === session) return elem.session;
      });

      return isOwner.id === id ? usr : {
        id:       usr.id,
        username: usr.username,
        avatar:   usr.avatar
      };
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
