/*
 * Swift Parallel Scripting Language (http://swift-lang.org)
 * Code from Java CoG Kit Project (see notice below) with modifications.
 *
 * Copyright 2005-2014 University of Chicago
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

//----------------------------------------------------------------------
//This code is developed as part of the Java CoG Kit project
//The terms of the license can be found at http://www.cogkit.org/license
//This message may not be removed or altered.
//----------------------------------------------------------------------

/*
 * Created on Nov 23, 2005
 */
package org.globus.cog.abstraction.impl.file.gridftp;

import org.globus.cog.util.Enumerated;
import org.globus.ftp.GridFTPSession;

public class DataChannelProtectionType extends Enumerated {
	public static final DataChannelProtectionType CLEAR = new DataChannelProtectionType("CLEAR",
			GridFTPSession.PROTECTION_CLEAR);
	public static final DataChannelProtectionType SAFE = new DataChannelProtectionType("SAFE",
			GridFTPSession.PROTECTION_SAFE);
	public static final DataChannelProtectionType CONFIDENTIAL = new DataChannelProtectionType(
			"CONFIDENTIAL", GridFTPSession.PROTECTION_CONFIDENTIAL);
	public static final DataChannelProtectionType PRIVATE = new DataChannelProtectionType(
			"PRIVATE", GridFTPSession.PROTECTION_PRIVATE);

	private DataChannelProtectionType(String literal, int value) {
		super(literal, value);
	}
}
