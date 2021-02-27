// pkg
const base64url = require('base64url');
const uuid4 = require('uuid4');

class Util {
	/**
	 * Generate ID deterministically with username.
	 *
	 * @param  {string} uname Username fed into base64url.
	 * @return  {string} Generated ID.
	 */
	GenId = (uname) => { return base64url(uname); };

	/**
	 * Generate session with UUID
	 *
	 * @return  {string} Generated session.
	 */
	GenSession = () => { return uuid4(); };
};

const util = new Util();
module.exports = {
	PATH:				'/api/v1/',				// default path
  GenId:			util.GenId,
  GenSession:	util.GenSession,
};
