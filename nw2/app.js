// require
const express = require('express');						// expr
const createUsr = require('./createUsr.js');	// new usr
const login = require('./login.js');					// login

// const var
const app = express();
const port = 3100;

app.use(express.json()); // expr to parse json

// listening log
app.listen(port, () => {
	console.log('started server at ', app.mountpath);
});

exports.usrmap = new Map(); // usr map, exported

const path = '/api/v1/'; // default path
// creating usr w/ POST on users/
app.post(path + 'users', createUsr);
// login w/ POST to login/
app.post(path + 'login', login);
