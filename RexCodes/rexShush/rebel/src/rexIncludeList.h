#ifndef RexIncludeList_H
#define RexIncludeList_H

void rebelApp::setIncludeList()
{
	_rexIncludeList.push_back("#include <stdio.h>\n");
	_rexIncludeList.push_back("#include <sys/types.h>\n");
	_rexIncludeList.push_back("#include \"../hdr/sys.h\"\n");
	_rexIncludeList.push_back("#include \"../hdr/cnf.h\"\n");
	_rexIncludeList.push_back("#include \"../hdr/proc.h\"\n");
	_rexIncludeList.push_back("#include \"../hdr/cdsp.h\"\n");
	_rexIncludeList.push_back("#include \"../hdr/idsp.h\"\n");
	_rexIncludeList.push_back("#include \"../hdr/buf.h\"\n");
	_rexIncludeList.push_back("#include \"../hdr/menu.h\"\n");
	_rexIncludeList.push_back("#include \"../hdr/state.h\"\n");
	_rexIncludeList.push_back("#include \"../hdr/ecode.h\"\n");
	_rexIncludeList.push_back("#include \"../hdr/device.h\"\n");
	_rexIncludeList.push_back("#include \"../hdr/int.h\"\n");
}

#endif
