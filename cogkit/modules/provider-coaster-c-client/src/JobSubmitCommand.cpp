/*
 * Swift Parallel Scripting Language (http://swift-lang.org)
 *
 * Copyright 2012-2014 University of Chicago
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "JobSubmitCommand.h"
#include "CoasterError.h"
#include <cstring>
#include <string>

using namespace Coaster;

using std::map;
using std::ostream;
using std::string;
using std::stringstream;
using std::vector;

void add(string& ss, const char* key, const string* value);
void add(string& ss, const char* key, const string& value);
void add(string& ss, const char* key, const char* value);
void add(string& ss, const char* key, const char* value, int n);
string toString(CoasterStagingMode);

string JobSubmitCommand::NAME("SUBMITJOB");

JobSubmitCommand::JobSubmitCommand(Job* pjob, const std::string& pconfigId): Command(&NAME) {
	job = pjob;
	configId = pconfigId;
}

void JobSubmitCommand::send(CoasterChannel* channel, CommandCallback* cb) {
	serialize();
	Command::send(channel, cb);
}

Job* JobSubmitCommand::getJob() {
	return job;
}

string JobSubmitCommand::getRemoteId() {
	if (!isReceiveCompleted() || isErrorReceived()) {
		throw CoasterError("getRemoteId called before reply was received");
	}
	string result;
	getInData()->at(0)->str(result);
	return result;
}

void JobSubmitCommand::serialize() {
	stringstream idSS;
	idSS << job->getIdentity();
	add(ss, "configid", configId);
	add(ss, "identity", idSS.str());
	add(ss, "executable", job->getExecutable());
	add(ss, "directory", job->getDirectory());

	add(ss, "stdin", job->getStdinLocation());
	add(ss, "stdout", job->getStdoutLocation());
	add(ss, "stderr", job->getStderrLocation());

	std::vector<StagingSetEntry>* stageins = job->getStageIns();
	if (stageins != NULL) {
		for (std::vector<StagingSetEntry>::iterator i = stageins->begin(); i != stageins->end(); ++i) {
			LogDebug << "stagein -- " << i->getSource() << endl << i->getDestination() << endl << toString(i->getMode()) << endl;
			LogInfo << "stagein -- " << i->getSource() << endl << i->getDestination() << endl << toString(i->getMode()) << endl;
			std::cout << "stagein -- " << i->getSource() << endl << i->getDestination() << endl << toString(i->getMode()) << endl;
			add(ss, "stagein",
					string(i->getSource()).append("\n").append(i->getDestination()).append("\n").append(toString(i->getMode())));
		}
	}
	else
{	LogDebug << "FAILURE TO READ IN FILES" << endl;
	std::cout << "FAILURE TO READ IN FILES" << endl;
	LogInfo << "FAILURE TO READ IN FILES" << endl;
}
	std::vector<StagingSetEntry>* stageouts = job->getStageOuts();
	if (stageouts != NULL) {
		for (std::vector<StagingSetEntry>::iterator i = stageouts->begin(); i != stageouts->end(); ++i) {
			LogDebug << "stageout -- " << i->getSource() << endl << i->getDestination() << endl << toString(i->getMode()) << endl;
			LogInfo << "stageout -- " << i->getSource() << endl << i->getDestination() << endl << toString(i->getMode()) << endl;
			std::cout << "stageout -- " << i->getSource() << endl << i->getDestination() << endl << toString(i->getMode()) << endl;
			add(ss, "stageout",
					string(i->getSource()).append("\n").append(i->getDestination()).append("\n").append(toString(i->getMode())));
		}
	}

	const vector<string*>& arguments = job->getArguments();
	for (vector<string*>::const_iterator i = arguments.begin(); i != arguments.end(); ++i) {
		add(ss, "arg", *i);
	}

	map<string, string>* env = job->getEnv();
	if (env != NULL) {
		for (map<string, string>::iterator i = env->begin(); i != env->end(); ++i) {
			add(ss, "env", string(i->first).append("=").append(i->second));
		}
	}

	map<string, string>* attributes = job->getAttributes();
	if (attributes != NULL) {
		for (map<string, string>::iterator i = attributes->begin(); i != attributes->end(); ++i) {
			add(ss, "attr", string(i->first).append("=").append(i->second));
		}
	}

	if (job->getJobManager().empty()) {
		LogDebug << "getJobManager == NULL, setting to :  fork "<< endl;
		add(ss, "jm", "fork");
	}
	else {
		const char *jm_string = (job->getJobManager()).c_str();
		LogDebug << "getJobManager != !NULL, setting to : "<< job->getJobManager() << endl;
		add(ss, "jm", jm_string);
	}
	addOutData(Buffer::wrap(ss));
}

void add(string& ss, const char* key, const string* value) {
	if (value != NULL) {
		add(ss, key, value->data(), value->length());
	}
}

void add(string& ss, const char* key, const string& value) {
	add(ss, key, value.data(), value.length());
}

void add(string& ss, const char* key, const char* value) {
	add(ss, key, value, -1);
}

void add(string& ss, const char* key, const char* value, int n) {
	if (value != NULL && n != 0) {
		ss.append(key);
		ss.append(1, '=');
		while (*value) {
			char c = *value;
			switch (c) {
				case '\n':
					ss.append(1, '\\');
					ss.append(1, 'n');
					break;
				case '\\':
					ss.append(1, '\\');
					ss.append(1, '\\');
					break;
				default:
					ss.append(1, c);
					break;
			}
			value++;
			n--;
		}
	}
	ss.append(1, '\n');
}

string toString(CoasterStagingMode m) {
	string mode;
	if(m == COASTER_STAGE_ALWAYS) mode = "1"; // "COASTER_STAGE_ALWAYS";
	else if(m == COASTER_STAGE_IF_PRESENT) mode = "2"; // "COASTER_STAGE_IF_PRESENT";
	else if(m == COASTER_STAGE_ON_ERROR) mode = "4"; // "COASTER_STAGE_ON_ERROR";
	else if(m == COASTER_STAGE_ON_SUCCESS) mode = "8"; // "COASTER_STAGE_ON_SUCCESS";
	else mode = "16"; //"COASTER_STAGE_NEVER";

	return mode;
}
