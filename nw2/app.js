// require
const express = require('express');
const base64url = require('base64url');
// const userRoute = require('./userRoute.js');

// const var
const app = express();
const port = 3100;

app.use(express.json()); // expr to parse json

// app.get('/api/v1/users/:arbitrary', userRoute)
app.listen(port, () => {
	console.log('started server at ', app.mountpath);
});

let usrmap = new Map(); // usr map

app.post('/api/v1/users', (req, res) => {
	// Create User
	// Path: /api/v1/users/
	// Verb: POST
	// Authenticated: No
	// Request Body:
	// •	username
	// •	password
	// •	avatar
	// Response Body:
	// •	id (the user ID)
	// •	username
	// •	password
	// •	avatar
	// Creates a new user, succeeding if that user did not already exist.

	// console.log(req.body);

	// gen id deterministically w/ usrname
	const reqId = base64url(req.body.username);

	// err status when dupl usr
	if (usrmap.has(reqId)) {
		res.status(409).json({error: 'dupl usr'});
		return;
	};

	// set new usr to map
	usrmap.set(reqId, {
		username: req.body.username,
		password: req.body.password,
		avatar: req.body.avatar
	});

	const createdUsr = {
		id: reqId,
		username: usrmap.get(reqId).username,
		password: usrmap.get(reqId).password,
		avatar: usrmap.get(reqId).avatar
	};

	// res w/ json & success status
	res.json(createdUsr);
	res.status(200);
});
