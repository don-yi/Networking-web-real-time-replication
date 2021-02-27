// require
const express = require('express');
const mongoCli = require('mongodb').MongoClient;
const redisCli = require('redis').createClient(6379, '127.0.0.1');

const usrCreate = require('./usr-create.js');		// new usr
const login = require('./usr-login.js');				// login
const usrById = require('./usr-by-id.js');			// retrieve usr by id
const usrByName = require('./usr-by-name.js');	// search usr by name
const usrUpdate = require('./usr-update.js');		// update

// const var
const app = express();
const port = 3100;
const path = '/api/v1/';

redisCli.on('error', (err) => { console.error(err); })
app.use(express.json());	// expr to parse json

// connect to mongo cli
const connectionString
    = 'mongodb://cs261-dennis-l:vptQDDQtAjf28CUy@54.151.57.127/cs261-dennis-l';
const mongoOptions = { useUnifiedTopology: true, authSource: "admin" };
mongoCli.connect( connectionString, mongoOptions, (err, mongoCli) => {
	if (err) { console.error(err); return; };

	// connect to my db
	const db = mongoCli.db('cs261-dennis-l');
	// get gam collection
	const usrCollection = db.collection('users');

	// creating usr w/ POST on users/
	usrCreate(app, usrCollection);
	// login w/ POST to login/
	login(app, usrCollection, redisCli);
	// retrieve usr by id w/ GET on users/:id
	usrById(app, usrCollection, redisCli);
	// search for a usr by name w/ GET on users
	app.get(path + 'users', usrByName);
	// update usr by id w/ PUT on users/:id
	app.put(path + 'users/:id', usrUpdate);
});

// listening
app.listen(port, () => {
	console.log('started server at ', app.mountpath);
});
