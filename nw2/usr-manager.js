class UserManager {
  // usr container
  allUsr = [];

  // find usr in container
  UsrFound = (id) => {
    for (const usr of this.allUsr) {
      if (usr.id == id) return true;
    };
    return false;
  };

  // gen new usr obj and push to container
  CreateUsr = (id, json) => {
    const newUsr = {
      id: id,
      username: json.username,
      password: json.password,
      avatar: json.avatar
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
};

const usrman = new UserManager();
module.exports = {
  UsrFound:  usrman.UsrFound,
  CreateUsr: usrman.CreateUsr,
  IsBadPw:   usrman.IsBadPw
};
