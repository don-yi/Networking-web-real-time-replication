const util = require('./util.js');

class UserManager {
  // container
  allUsr = [];
  allSession = [];

  /**
   * Returns boolean if user with id exists.
   *
   * @param  {string} id User ID to find in user container.
   * @return  {boolean} True if found, false otherwise.
   */
  UsrFound = (id) => this.allUsr.find((usr) => {
    return (usr.id === id) ? true : false;
  });

  /**
   * Generate new user object and push to container.
   *
   * @param  {string} id User ID to add to user info.
   * @param  {object} uinfo User info.
   * @return  {object} New user object created in container.
   */
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

  /**
   * Check for password match.
   *
   * @param  {object} uinfo User info.
   * @return  {boolean} True if bad match, false otherwise.
   */
  IsBadPw = (uinfo) => this.allUsr.find((usr) => {
    if (usr.username === uinfo.username)
      return (usr.password != uinfo.password) ? true : false;
  });

  /**
   * Create new session.
   *
   * @param  {string} id User ID to add to session info.
   * @return  {object} New session object created in container.
   */
  CreateSession = (id) => {
    const sessionObj = {
      session:  util.GenSession(),
      id:       id
    };
    this.allSession.push(sessionObj);
    return sessionObj;
  };

  /**
   * Get matching session.
   *
   * @param  {string} session UUID session to find in container.
   * @return  {object} Session object found in container.
   */
  GetMatchingSessionId = (session) => {
    return this.allSession.find((sessionId) => {
      if (sessionId.session === session) return sessionId;
    });
  };

  /**
   * Retrieve user object depends on whether owner or non owner.
   *
   * @param  {string} id User ID to compare ID in container.
   * @param  {string} session UUID session to find in container.
   * @return  {object} User object with password if owner, without otherwise.
   */
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

  /**
   * Update user from container depends on whether owner / non owner.
   *
   * @param  {object} newInfo New user info object to update to.
   * @param  {string} session UUID session to find in container.
   * @param  {string} id User ID to compare ID in container.
   * @return  {object} Updated user object if owner, undefined otherwise.
   */
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
