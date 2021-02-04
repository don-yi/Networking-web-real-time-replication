// require
const base64url = require('base64url');
const uuid4 = require('uuid4');

class Util {
	// gen id deterministically w/ usrname
	GenId = (uname) => { return base64url(uname); };

	// gen session w/ uuid
	GenSession = () => { return uuid4(); };
};

const util = new Util();
module.exports = {
  GenId:      util.GenId,
  GenSession: util.GenSession
};
