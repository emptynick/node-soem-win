var soem = require('./build/Release/node-soem');
var master = soem.NodeSoemMaster("\\Device\\NPF_{E093CA7E-BA42-4884-B069-5B5AD03D481E}");

master.init();

console.log(master.getAdapters());