/*
   Copyright 2016 Yuki KAN

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
/// <reference path="../typings/index.d.ts" />
"use strict";
const child_process_1 = require("child_process");
const _1 = require("./Mirakurun/_");
const status_1 = require("./Mirakurun/status");
const Event_1 = require("./Mirakurun/Event");
const Tuner_1 = require("./Mirakurun/Tuner");
const Channel_1 = require("./Mirakurun/Channel");
const Service_1 = require("./Mirakurun/Service");
const Program_1 = require("./Mirakurun/Program");
const Server_1 = require("./Mirakurun/Server");
const config = require("./Mirakurun/config");
const log = require("./Mirakurun/log");
process.title = "Mirakurun: Server";
process.on("uncaughtException", err => {
    ++status_1.default.errorCount.uncaughtException;
    console.error(err.stack);
});
setEnv("SERVER_CONFIG_PATH", "/usr/local/etc/mirakurun/server.yml");
setEnv("TUNERS_CONFIG_PATH", "/usr/local/etc/mirakurun/tuners.yml");
setEnv("CHANNELS_CONFIG_PATH", "/usr/local/etc/mirakurun/channels.yml");
setEnv("SERVICES_DB_PATH", "/usr/local/var/db/mirakurun/services.json");
setEnv("PROGRAMS_DB_PATH", "/usr/local/var/db/mirakurun/programs.json");
if (process.platform === "linux") {
    child_process_1.execSync(`renice -n -10 -p ${process.pid}`);
    child_process_1.execSync(`ionice -c 1 -n 7 -p ${process.pid}`);
}
_1.default.config.server = config.loadServer();
_1.default.config.channels = config.loadChannels();
_1.default.config.tuners = config.loadTuners();
if (typeof _1.default.config.server.logLevel === "number") {
    log.logLevel = _1.default.config.server.logLevel;
}
new Event_1.default();
new Tuner_1.default();
new Channel_1.default();
new Service_1.default();
new Program_1.default();
new Server_1.default();
function setEnv(name, value) {
    process.env[name] = process.env[name] || value;
}

//# sourceMappingURL=server.js.map
