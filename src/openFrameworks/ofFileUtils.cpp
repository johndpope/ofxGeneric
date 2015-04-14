#include "ofFileUtils.h"
#ifndef TARGET_WIN32
 #include <pwd.h>
#endif

#include "ofUtils.h"


#ifdef TARGET_OSX
 #include <mach-o/dyld.h>       /* _NSGetExecutablePath */
 #include <limits.h>        /* PATH_MAX */
#endif


//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
// -- ofBuffer
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

//--------------------------------------------------
ofBuffer::ofBuffer(){
	nextLinePos = 0;
	buffer.resize(1);
}

//--------------------------------------------------
ofBuffer::ofBuffer(const char * buffer, unsigned int size){
	set(buffer, size);
}

//--------------------------------------------------
ofBuffer::ofBuffer(const string & text){
	set(text);
}

//--------------------------------------------------
ofBuffer::ofBuffer(istream & stream){
	set(stream);
}

//--------------------------------------------------
ofBuffer::ofBuffer(const ofBuffer & buffer_){
	buffer = buffer_.buffer;
	nextLinePos = buffer_.nextLinePos;
}

//--------------------------------------------------
ofBuffer::~ofBuffer(){
	clear();
}

//--------------------------------------------------
bool ofBuffer::set(istream & stream){
	clear();
	if(stream.bad()){
		return false;
	}

	char aux_buffer[1024];
	std::streamsize size = 0;
	stream.read(aux_buffer, 1024);
	std::streamsize n = stream.gcount();
	while(n > 0){
		// we resize to size+1 initialized to 0 to have a 0 at the end for strings
		buffer.resize(size + n + 1, 0);
		memcpy(&(buffer[0]) + size, aux_buffer, n);
		size += n;
		if(stream){
			stream.read(aux_buffer, 1024);
			n = stream.gcount();
		}
		else{n = 0;
		}
	}
	return true;
}

//--------------------------------------------------
bool ofBuffer::writeTo(ostream & stream) const {
	if(stream.bad()){
		return false;
	}
	stream.write(&(buffer[0]), buffer.size() - 1);
	return true;
}

//--------------------------------------------------
void ofBuffer::set(const char * _buffer, unsigned int _size){
	buffer.assign(_buffer,_buffer+_size);
	buffer.resize(buffer.size()+1);
	buffer.back() = 0;
}

//--------------------------------------------------
void ofBuffer::set(const string & text){
	set(text.c_str(),text.size());
}

//--------------------------------------------------
void ofBuffer::append(const string& _buffer){
	append(_buffer.c_str(), _buffer.size());
}

//--------------------------------------------------
void ofBuffer::append(const char * _buffer, unsigned int _size){
	buffer.insert(buffer.end()-1,_buffer,_buffer+_size);
	buffer.back() = 0;
}

//--------------------------------------------------
void ofBuffer::clear(){
	buffer.resize(1);
	nextLinePos = 0;
}

//--------------------------------------------------
void ofBuffer::allocate(long _size){
	clear();
	buffer.resize(_size);
}

//--------------------------------------------------
char *ofBuffer::getBinaryBuffer(){
	if(buffer.empty()){
		return NULL;
	}
	return &buffer[0];
}

//--------------------------------------------------
const char *ofBuffer::getBinaryBuffer() const {
	if(buffer.empty()){
		return "";
	}
	return &buffer[0];
}

//--------------------------------------------------
string ofBuffer::getText() const {
	if(buffer.empty()){
		return "";
	}
	return &buffer[0];
}

//--------------------------------------------------
ofBuffer::operator string() const {
	return getText();
}

//--------------------------------------------------
ofBuffer & ofBuffer::operator=(const string & text){
	set(text);
	return *this;
}

//--------------------------------------------------
long ofBuffer::size() const {
	if(buffer.empty()){
		return 0;
	}
	//we always add a 0 at the end to avoid problems with strings
	return buffer.size() - 1;
}

//--------------------------------------------------
string ofBuffer::getNextLine(){
	if(buffer.empty() || (int)(buffer.size() - 1) == nextLinePos){
		return "";
	}
	long currentLinePos = nextLinePos;
	bool lineEndWasCR = false;
	while(nextLinePos < (int)buffer.size() - 1 && buffer[nextLinePos] != '\n'){
		if(buffer[nextLinePos] != '\r'){
			nextLinePos++;
		}
		else{
			lineEndWasCR = true;
			break;
		}
	}
	string line(getBinaryBuffer() + currentLinePos, nextLinePos - currentLinePos);
	if(nextLinePos < (int)(buffer.size() - 1)){
		nextLinePos++;
	}
	// if lineEndWasCR check for CRLF
	if(lineEndWasCR && nextLinePos < (int)(buffer.size() - 1) && buffer[nextLinePos] == '\n'){
		nextLinePos++;
	}
	return line;
}

//--------------------------------------------------
string ofBuffer::getFirstLine(){
	resetLineReader();
	return getNextLine();
}

//--------------------------------------------------
bool ofBuffer::isLastLine(){
	return (int)(buffer.size() - 1) == nextLinePos;
}

//--------------------------------------------------
void ofBuffer::resetLineReader(){
	nextLinePos = 0;
}

//--------------------------------------------------
ostream & operator<<(ostream & ostr, const ofBuffer & buf){
	buf.writeTo(ostr);
	return ostr;
}

//--------------------------------------------------
istream & operator>>(istream & istr, ofBuffer & buf){
	buf.set(istr);
	return istr;
}

//--------------------------------------------------
ofBuffer ofBufferFromFile(const string & path, bool binary){
	ios_base::openmode mode = binary ? ifstream::binary : ios_base::in;
	ifstream istr(ofToDataPath(path, true).c_str(), mode);
	ofBuffer buffer(istr);
	istr.close();
	return buffer;
}

//--------------------------------------------------
bool ofBufferToFile(const string & path, ofBuffer & buffer, bool binary){
	ios_base::openmode mode = binary ? ofstream::binary : ios_base::out;
	ofstream ostr(ofToDataPath(path, true).c_str(), mode);
	bool ret = buffer.writeTo(ostr);
	ostr.close();
	return ret;
}

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
// -- ofFile
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------


/* TZLA-619 // #include "Poco/Util/FilesystemConfiguration.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/StringTokenizer.h"
#include "Poco/Exception.h"
#include "Poco/FileStream.h"
#include "Poco/String.h"

using namespace Poco;*/

//------------------------------------------------------------------------------------------------------------
ofFile::ofFile(){
	mode = Reference;
}

//------------------------------------------------------------------------------------------------------------
ofFile::ofFile(string path, Mode mode, bool binary){
	open(path, mode, binary);
}

//-------------------------------------------------------------------------------------------------------------
ofFile::~ofFile(){
	close();
}

//-------------------------------------------------------------------------------------------------------------
ofFile::ofFile(const ofFile & mom){
	copyFrom(mom);
}

//-------------------------------------------------------------------------------------------------------------
ofFile & ofFile::operator=(const ofFile & mom){
	copyFrom(mom);
	return *this;
}

//-------------------------------------------------------------------------------------------------------------
void ofFile::copyFrom(const ofFile & mom){
	if(&mom != this){
		Mode new_mode = mom.mode;
		if(new_mode != Reference && new_mode != ReadOnly){
			new_mode = ReadOnly;
			// TZLA-619 // ofLog(OF_LOG_WARNING, "ofFile: trying to copy a write file, opening copy as read only");
		}
		// TZLA-619 // open(mom.myFile.path(), new_mode);
	}
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::openStream(Mode _mode, bool binary){
	mode = _mode;
	ios_base::openmode binary_mode = binary ? ios::binary : (ios_base::openmode)0;
	switch(_mode){
	 case Reference:
		 return true;
		 break;

	 case ReadOnly:
		 if(exists()){
			 fstream::open(path().c_str(), ios::in | binary_mode);
		 }
		 break;

	 case WriteOnly:
		 fstream::open(path().c_str(), ios::out | binary_mode);
		 break;

	 case ReadWrite:
		 fstream::open(path().c_str(), ios_base::in | ios_base::out | binary_mode);
		 break;

	 case Append:
		 fstream::open(path().c_str(), ios::out | ios::app | binary_mode);
		 break;
	}
	return fstream::good();
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::open(string _path, Mode _mode, bool binary){
	close();
	// TZLA-619 // myFile = File(ofToDataPath(_path));
	return openStream(_mode, binary);
}

//-------------------------------------------------------------------------------------------------------------
bool ofFile::changeMode(Mode _mode, bool binary){
	if(_mode != mode){
		string _path = path();
		close();
		// TZLA-619 // myFile = File(_path);
		return openStream(_mode, binary);
	}
	else{
		return true;
	}
}

//-------------------------------------------------------------------------------------------------------------
bool ofFile::isWriteMode(){
	return mode != ReadOnly;
}

//-------------------------------------------------------------------------------------------------------------
void ofFile::close(){
	// TZLA-619 // myFile = File();
	fstream::close();
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::create(){
	bool success = false;

	/* TZLA-619 // if(myFile.path() != ""){
		try{
			success = myFile.createFile();
		}
		catch(Poco::Exception & except){
			ofLog(OF_LOG_ERROR, "ofFile::copyTo - unable to copy");
			return false;
		}
	}*/

	return success;
}

//------------------------------------------------------------------------------------------------------------
ofBuffer ofFile::readToBuffer(){
	/* TZLA-619 // if(myFile.path() == "" || myFile.exists() == false){
		return ofBuffer();
	}*/

	return ofBuffer(*this);
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::writeFromBuffer(ofBuffer & buffer){
	/* TZLA-619 // if(myFile.path() == ""){
		return false;
	}*/
	if(!isWriteMode()){
		// TZLA-619 // ofLog(OF_LOG_ERROR, "ofFile: trying to a file opened as read only");
	}
	*this << buffer;
	return true;
}

//------------------------------------------------------------------------------------------------------------
filebuf *ofFile::getFileBuffer() const {
	return rdbuf();
}


//-- poco wrappers
//------------------------------------------------------------------------------------------------------------
bool ofFile::exists() const {
	if(path().empty()){
		return false;
	}
	try{
		// TZLA-619 // return myFile.exists();
        return false;
	}
	catch(...){
		return false;
	}
}

//------------------------------------------------------------------------------------------------------------
string ofFile::path() const {
	// TZLA-619 // return myFile.path();
    return "";
}

//------------------------------------------------------------------------------------------------------------
string ofFile::getExtension() const {
	return ofFilePath::getFileExt(path());
}

//------------------------------------------------------------------------------------------------------------
string ofFile::getFileName() const {
	return ofFilePath::getFileName(path());
}

//------------------------------------------------------------------------------------------------------------
string ofFile::getBaseName() const {
	return ofFilePath::removeExt(ofFilePath::getFileName(path()));
}

//------------------------------------------------------------------------------------------------------------
string ofFile::getEnclosingDirectory() const {
	return ofFilePath::getEnclosingDirectory(path());
}

//------------------------------------------------------------------------------------------------------------
string ofFile::getAbsolutePath() const {
	return ofFilePath::getAbsolutePath(path());
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::canRead() const {
	// TZLA-619 // return myFile.canRead();
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::canWrite() const {
	// TZLA-619 // return myFile.canWrite();
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::canExecute() const {
	// TZLA-619 // return myFile.canExecute();
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::isFile() const {
	// TZLA-619 // return myFile.isFile();
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::isLink() const {
	// TZLA-619 // return myFile.isLink();
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::isDirectory() const {
	// TZLA-619 // return myFile.isDirectory();
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::isDevice() const {
	// TZLA-619 // return myFile.isDevice();
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::isHidden() const {
	// TZLA-619 // return myFile.isHidden();
    return false;
}

//------------------------------------------------------------------------------------------------------------
void ofFile::setWriteable(bool flag = true){
	// TZLA-619 // myFile.setWriteable(flag);
}

//------------------------------------------------------------------------------------------------------------
void ofFile::setReadOnly(bool flag = true){
	// TZLA-619 // myFile.setReadOnly(flag);
}

//------------------------------------------------------------------------------------------------------------
void ofFile::setExecutable(bool flag = true){
	// TZLA-619 // myFile.setExecutable(flag);
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::copyTo(string path, bool bRelativeToData, bool overwrite){
	/* TZLA-619 // if(path.empty() || !myFile.exists()){
		ofLog(OF_LOG_ERROR, "ofFile::copyTo: trying to copy a non existing file");
		return false;
	}

	if(bRelativeToData){
		path = ofToDataPath(path);
	}
	if(ofFile::doesFileExist(path, bRelativeToData)){
		if(overwrite){
			ofFile::removeFile(path, bRelativeToData);
		}
		else{
			ofLog(OF_LOG_WARNING, "ofFile::copyTo dest file already exists, use bool overwrite to overwrite dest file");
		}
	}

	try{
		myFile.copyTo(path);
	}
	catch(Poco::Exception & except){
		ofLog(OF_LOG_ERROR, "ofFile::copyTo - unable to copy");
		return false;
	}

	return true;*/
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::moveTo(string path, bool bRelativeToData, bool overwrite){
	/* TZLA-619 // if(path.empty() || !myFile.exists()){
		return false;
	}

	if(bRelativeToData){
		path = ofToDataPath(path);
	}
	if(ofFile::doesFileExist(path, bRelativeToData)){
		if(overwrite){
			ofFile::removeFile(path, bRelativeToData);
		}
		else{
			ofLog(OF_LOG_WARNING, "ofFile::moveTo dest file already exists, use bool overwrite to overwrite dest file");
		}
	}

	try{
		myFile.moveTo(path);
	}
	catch(Poco::Exception & except){
		ofLog(OF_LOG_ERROR, "ofFile::moveTo - unable to move");
		return false;
	}

	return true;*/
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::renameTo(string path, bool bRelativeToData, bool overwrite){
	/* TZLA-619 // if(path.empty() || !myFile.exists()){
		return false;
	}

	if(bRelativeToData){
		path = ofToDataPath(path);
	}
	if(ofFile::doesFileExist(path, bRelativeToData)){
		if(overwrite){
			ofFile::removeFile(path, bRelativeToData);
		}
		else{
			ofLog(OF_LOG_WARNING, "ofFile::renameTo dest file already exists, use bool overwrite to overwrite dest file");
			return false;
		}
	}

	try{
		myFile.renameTo(path);
	}
	catch(Poco::Exception & except){
		ofLog(OF_LOG_ERROR, "ofFile::renameTo - unable to rename");
		return false;
	}

	return true;*/
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::remove(bool recursive){
	/* TZLA-619 // if(myFile.path().empty() || !myFile.exists()){
		return false;
	}

	try{
		myFile.remove(recursive);
	}
	catch(Poco::Exception & except){
		ofLog(OF_LOG_ERROR, "ofFile::remove - unable to remove file/folder");
		ofLog(OF_LOG_ERROR, "ofFile::remove - " + except.displayText());
		return false;
	}

	return true;*/
    return false;
}

//------------------------------------------------------------------------------------------------------------
uint64_t ofFile::getSize() const {
	// TZLA-619 // return myFile.getSize();
    return -1;
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::operator==(const ofFile & file) const {
	return getAbsolutePath() == file.getAbsolutePath();
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::operator!=(const ofFile & file) const {
	return getAbsolutePath() != file.getAbsolutePath();
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::operator<(const ofFile & file) const {
	return getAbsolutePath() < file.getAbsolutePath();
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::operator<=(const ofFile & file) const {
	return getAbsolutePath() <= file.getAbsolutePath();
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::operator>(const ofFile & file) const {
	return getAbsolutePath() > file.getAbsolutePath();
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::operator>=(const ofFile & file) const {
	return getAbsolutePath() >= file.getAbsolutePath();
}

//------------------------------------------------------------------------------------------------------------
// ofFile Static Methods
//------------------------------------------------------------------------------------------------------------

bool ofFile::copyFromTo(string pathSrc, string pathDst, bool bRelativeToData,  bool overwrite){
	/* TZLA-619 // if(bRelativeToData){
		pathSrc = ofToDataPath(pathSrc);
	}
	if(bRelativeToData){
		pathDst = ofToDataPath(pathDst);
	}

	if(!ofFile::doesFileExist(pathSrc, bRelativeToData)){
		ofLog(OF_LOG_ERROR, "ofFile::copyFromTo source file/folder doesn't exist: %s", pathSrc.c_str());
		return false;
	}
	
	if(ofFile::doesFileExist(pathDst, bRelativeToData)){
		if(overwrite){
			ofFile::removeFile(pathDst, bRelativeToData);
		}
		else{
			ofLog(OF_LOG_WARNING, "ofFile::copyFromTo destination file/folder exists, use bool overwrite if you want to overwrite destination file/folder");
		}
	}

	File fileSrc(pathSrc);
	try{
		fileSrc.copyTo(pathDst);
	}
	catch(Poco::Exception & except){
		ofLog(OF_LOG_ERROR, "copyFromTo - unable to copy");
		return false;
	}
	return true;*/
    return false;
}

//be careful with slashes here - appending a slash when moving a folder will causes mad headaches
//------------------------------------------------------------------------------------------------------------
bool ofFile::moveFromTo(string pathSrc, string pathDst, bool bRelativeToData, bool overwrite){
	/* TZLA-619 // if(bRelativeToData){
		pathSrc = ofToDataPath(pathSrc);
	}
	if(bRelativeToData){
		pathDst = ofToDataPath(pathDst);
	}

	if(!ofFile::doesFileExist(pathSrc, bRelativeToData)){
		ofLog(OF_LOG_ERROR, "ofFile::moveFromTo source file/folder doesn't exist: %s", pathSrc.c_str());	
		return false;
	}
		
	if(ofFile::doesFileExist(pathDst, bRelativeToData)){
		if(overwrite){
			ofFile::removeFile(pathDst, bRelativeToData);
		}
		else{
			ofLog(OF_LOG_WARNING, "ofFile::moveFromTo destination file/folder exists, use bool overwrite if you want to overwrite destination file/folder");
		}
	}

	File fileSrc(pathSrc);
	try{
		fileSrc.moveTo(pathDst);
	}
	catch(Poco::Exception & except){
		ofLog(OF_LOG_ERROR, "moveFromTo - unable to move");
		return false;
	}
	return true;*/
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::doesFileExist(string fPath,  bool bRelativeToData){
	/* TZLA-619 // if(bRelativeToData){
		fPath = ofToDataPath(fPath);
	}
	File file(fPath);
	return !fPath.empty() && file.exists();*/
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool ofFile::removeFile(string path, bool bRelativeToData){
	/* TZLA-619 // if(bRelativeToData){
		path = ofToDataPath(path);
	}
	File file(path);
	try{
		file.remove();
	}
	catch(Poco::Exception & except){
		ofLog(OF_LOG_ERROR, "removeFile - file could not be deleted");
		ofLog(OF_LOG_ERROR, "removeFile - " + except.displayText());
		return false;
	}
	return true;*/
    return false;
}

/* TZLA-619 // Poco::File & ofFile::getPocoFile(){
	return myFile;
}*/

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
// -- ofDirectory
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------


//----------------------------------------
// Simple example of a boolean function that can be used
// with ofRemove to remove items from vectors.
bool hiddenFile(ofFile file){
	return file.isHidden();
}

//----------------------------------------
// Advanced example of a class that has a boolean function
// that can be used with ofRemove to remove items from vectors.
class ExtensionComparator : public unary_function<ofFile, bool> {
	public:
		vector<string> * extensions;
		inline bool operator()(const ofFile & file){
			/* TZLA-619 // Path curPath(file.path());
			string curExtension = toLower(curPath.getExtension());
			return !ofContains(*extensions, curExtension);*/
            return false;
		}
};

//------------------------------------------------------------------------------------------------------------
ofDirectory::ofDirectory(){
	showHidden = false;
}

//------------------------------------------------------------------------------------------------------------
ofDirectory::ofDirectory(string path){
	showHidden = false;
	open(path);
}

//------------------------------------------------------------------------------------------------------------
void ofDirectory::open(string path){
	path = ofFilePath::getPathForDirectory(path);
	originalDirectory = path;
	files.clear();
	// TZLA-619 // myDir = File(ofToDataPath(path));
}

//------------------------------------------------------------------------------------------------------------
void ofDirectory::close(){
	// TZLA-619 // myDir = File();
}

//------------------------------------------------------------------------------------------------------------
bool ofDirectory::create(bool recursive){

	/* TZLA-619 // if(myDir.path() != ""){
		try{
			if(recursive){
				myDir.createDirectories();
			}
			else{myDir.createDirectory();
			}
		}
		catch(Poco::Exception & except){
			ofLog(OF_LOG_ERROR, "ofDirectory::create - unable to create directory");
			return false;
		}
	}

	return true;*/
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool ofDirectory::exists() const {
	// TZLA-619 // return myDir.exists();
    return false;
}

//------------------------------------------------------------------------------------------------------------
string ofDirectory::path() const {
	// TZLA-619 // return myDir.path();
    return "";
}

//------------------------------------------------------------------------------------------------------------
string ofDirectory::getAbsolutePath() const {
	return ofFilePath::getAbsolutePath(path());
}

//------------------------------------------------------------------------------------------------------------
bool ofDirectory::isHidden() const {
	// TZLA-619 // return myDir.isHidden();
    return false;
}

//------------------------------------------------------------------------------------------------------------
void ofDirectory::setWriteable(bool flag = true){
	// TZLA-619 // myDir.setWriteable(flag);
}

//------------------------------------------------------------------------------------------------------------
void ofDirectory::setReadOnly(bool flag = true){
	// TZLA-619 // myDir.setReadOnly(flag);
}

//------------------------------------------------------------------------------------------------------------
void ofDirectory::setExecutable(bool flag = true){
	// TZLA-619 // myDir.setExecutable(flag);
}

//------------------------------------------------------------------------------------------------------------
void ofDirectory::setShowHidden(bool showHidden){
	this->showHidden = showHidden;
}

//------------------------------------------------------------------------------------------------------------
bool ofDirectory::isDirectory() const {
	// TZLA-619 // return myDir.isDirectory();
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool ofDirectory::copyTo(string path, bool bRelativeToData, bool overwrite){
	/* TZLA-619 // if(myDir.path().empty() || !myDir.exists()){
		return false;
	}

	if(bRelativeToData){
		path = ofToDataPath(path, bRelativeToData);
	}
	if(ofDirectory::doesDirectoryExist(path, bRelativeToData)){
		if(overwrite){
			ofDirectory::removeDirectory(path, true, bRelativeToData);
		}
		else{
			ofLog(OF_LOG_WARNING, "ofDirectory::copyTo dest folder already exists, use bool overwrite to overwrite dest folder");
		}
	}

	try{
		myDir.copyTo(path);
	}
	catch(Poco::Exception & except){
		ofLog(OF_LOG_ERROR, "ofDirectory::copyTo - unable to copy");
		return false;
	}

	return true;*/
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool ofDirectory::moveTo(string path,  bool bRelativeToData, bool overwrite){
	/* TZLA-619 // if(myDir.path().empty() || !myDir.exists()){
		return false;
	}

	if(bRelativeToData){
		path = ofToDataPath(path, bRelativeToData);
	}
	if(ofDirectory::doesDirectoryExist(path, bRelativeToData)){
		if(overwrite){
			ofDirectory::removeDirectory(path, true, bRelativeToData);
		}
		else{
			ofLog(OF_LOG_WARNING, "ofDirectory::moveTo dest folder already exists, use bool overwrite to overwrite dest folder");
		}
	}

	try{
		myDir.moveTo(path);
	}
	catch(Poco::Exception & except){
		ofLog(OF_LOG_ERROR, "ofDirectory::moveTo - unable to move");
		return false;
	}

	return true;*/
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool ofDirectory::renameTo(string path, bool bRelativeToData, bool overwrite){
	/* TZLA-619 // if(myDir.path().empty() || !myDir.exists()){
		return false;
	}

	if(bRelativeToData){
		path = ofToDataPath(path);
	}
	if(ofDirectory::doesDirectoryExist(path, bRelativeToData)){
		if(overwrite){
			ofDirectory::removeDirectory(path, true, bRelativeToData);
		}
		else{
			ofLog(OF_LOG_WARNING, "ofDirectory::renameTo dest folder already exists, use bool overwrite to overwrite dest folder");
			return false;
		}
	}

	try{
		myDir.renameTo(path);
	}
	catch(Poco::Exception & except){
		ofLog(OF_LOG_ERROR, "ofDirectory::renameTo - unable to rename");
		return false;
	}

	return true;*/
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool ofDirectory::remove(bool recursive){
	/* TZLA-619 // if(path().empty() || !myDir.exists()){
		return false;
	}

	try{
		myDir.remove(recursive);
	}
	catch(Poco::Exception & except){
		ofLog(OF_LOG_ERROR, "ofDirectory::remove - unable to remove file/folder");
		return false;
	}

	return true;*/
    return false;
}

//------------------------------------------------------------------------------------------------------------
void ofDirectory::allowExt(string extension){
	/* TZLA-619 // if(extension == "*"){
		ofLog(OF_LOG_WARNING, "ofDirectoryLister::allowExt() the extension * is deprecated");
	}
	extensions.push_back(toLower(extension));*/
}

//------------------------------------------------------------------------------------------------------------
int ofDirectory::listDir(string directory){
	open(directory);
	return listDir();
}

//------------------------------------------------------------------------------------------------------------
int ofDirectory::listDir(){
	/* TZLA-619 // Path base(path());
	if(!path().empty() && myDir.exists()){
		// File::list(vector<File>) is broken on windows as of march 23, 2011...
		// so we need to use File::list(vector<string>) and build a vector<File>
		// in the future the following can be replaced width: cur.list(files);
		vector<string>fileStrings;
		myDir.list(fileStrings);
		for(int i = 0; i < (int)fileStrings.size(); i++){
			Path curPath(originalDirectory);
			curPath.setFileName(fileStrings[i]);
			files.push_back(ofFile(curPath.toString(), ofFile::Reference));
		}

		if(!showHidden){
			ofRemove(files, hiddenFile);
		}

		if(!extensions.empty() && !ofContains(extensions, (string)"*")){
			ExtensionComparator extensionFilter;
			extensionFilter.extensions = &extensions;
			ofRemove(files, extensionFilter);
		}

		// TODO: if(ofCheckLogLevel(OF_LOG_VERBOSE)) {
		for(int i = 0; i < (int)size(); i++){
			ofLog(OF_LOG_VERBOSE, "\t" + getName(i));
		}
		ofLog(OF_LOG_VERBOSE, "ofDirectoryLister::listDirectory() listed " + ofToString(size()) + " files in " + originalDirectory);
		// }
	}
	else{
		ofLog(OF_LOG_ERROR, "ofDirectoryLister::listDirectory() error opening directory " + originalDirectory);
	}

	return size();*/
    return -1;
}

//------------------------------------------------------------------------------------------------------------
string ofDirectory::getOriginalDirectory(){
	return originalDirectory;
}

//------------------------------------------------------------------------------------------------------------
string ofDirectory::getName(unsigned int position){
	/* TZLA-619 // Path cur(files.at(position).path());
	return cur.getFileName();*/
    return "";
}

//------------------------------------------------------------------------------------------------------------
string ofDirectory::getPath(unsigned int position){
	return originalDirectory + getName(position);
}

//------------------------------------------------------------------------------------------------------------
ofFile ofDirectory::getFile(unsigned int position, ofFile::Mode mode, bool binary){
	ofFile file = files[position];
	file.changeMode(mode, binary);
	return file;
}

ofFile ofDirectory::operator[](unsigned int position){
	return getFile(position);
}

//------------------------------------------------------------------------------------------------------------
vector<ofFile>ofDirectory::getFiles(){
	return files;
}

//------------------------------------------------------------------------------------------------------------
bool ofDirectory::getShowHidden(){
	return showHidden;
}

//------------------------------------------------------------------------------------------------------------
void ofDirectory::reset(){
	close();
}

//------------------------------------------------------------------------------------------------------------
void ofDirectory::sort(){
	ofSort(files);
}

//------------------------------------------------------------------------------------------------------------
unsigned int ofDirectory::size(){
	return files.size();
}

//------------------------------------------------------------------------------------------------------------
int ofDirectory::numFiles(){
	return size();
}

//------------------------------------------------------------------------------------------------------------
// ofDirectory Static Methods
//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
bool ofDirectory::removeDirectory(string path, bool deleteIfNotEmpty, bool bRelativeToData){
	/* TZLA-619 // if(bRelativeToData){
		path = ofToDataPath(path);
	}
	File file(path);
	try{
		file.remove(deleteIfNotEmpty);
	}
	catch(Poco::Exception & except){
		ofLog(OF_LOG_ERROR, "deleteDirectory - folder could not be deleted");
		return false;
	}
	return true;*/
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool ofDirectory::createDirectory(string dirPath, bool bRelativeToData, bool recursive){
	/* TZLA-619 // if(bRelativeToData){
		dirPath = ofToDataPath(dirPath);
	}

	File file(dirPath);
	bool success = false;
	try{
		if(!recursive){
			success = file.createDirectory();
		}
		else{
			file.createDirectories();
			success = true;
		}
	}
	catch(Poco::Exception & except){
		ofLog(OF_LOG_ERROR, "createDirectory - directory could not be created");
		return false;
	}

	if(!success){
		ofLog(OF_LOG_WARNING, "createDirectory - directory already exists");
		success = true;
	}

	return success;*/
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool ofDirectory::doesDirectoryExist(string dirPath, bool bRelativeToData){
	/* TZLA-619 // if(bRelativeToData){
		dirPath = ofToDataPath(dirPath);
	}
	File file(dirPath);
	return file.exists();*/
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool ofDirectory::isDirectoryEmpty(string dirPath, bool bRelativeToData){
	/* TZLA-619 // if(bRelativeToData){
		dirPath = ofToDataPath(dirPath);
	}
	File file(dirPath);
	if(!dirPath.empty() && file.exists() && file.isDirectory()){
		vector<string>contents;
		file.list(contents);
		if(contents.size() == 0){
			return true;
		}
	}*/
	return false;
}

/* TZLA-619 // //------------------------------------------------------------------------------------------------------------
Poco::File & ofDirectory::getPocoFile(){
	return myDir;
}*/

//------------------------------------------------------------------------------------------------------------
bool ofDirectory::operator==(const ofDirectory & dir){
	return getAbsolutePath() == dir.getAbsolutePath();
}

//------------------------------------------------------------------------------------------------------------
bool ofDirectory::operator!=(const ofDirectory & dir){
	return getAbsolutePath() != dir.getAbsolutePath();
}

//------------------------------------------------------------------------------------------------------------
bool ofDirectory::operator<(const ofDirectory & dir){
	return getAbsolutePath() < dir.getAbsolutePath();
}

//------------------------------------------------------------------------------------------------------------
bool ofDirectory::operator<=(const ofDirectory & dir){
	return getAbsolutePath() <= dir.getAbsolutePath();
}

//------------------------------------------------------------------------------------------------------------
bool ofDirectory::operator>(const ofDirectory & dir){
	return getAbsolutePath() > dir.getAbsolutePath();
}

//------------------------------------------------------------------------------------------------------------
bool ofDirectory::operator>=(const ofDirectory & dir){
	return getAbsolutePath() >= dir.getAbsolutePath();
}

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
// -- ofFilePath
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
string ofFilePath::addLeadingSlash(string path){
	if(path.length() > 0){
		if(path[0] != '/'){
			path = "/" + path;
		}
	}
	return path;
}

//------------------------------------------------------------------------------------------------------------
string ofFilePath::addTrailingSlash(string path){
	if(path.length() > 0){
		if(path[path.length() - 1] != '/'){
			path += "/";
		}
	}
	return path;
}


//------------------------------------------------------------------------------------------------------------
string ofFilePath::getFileExt(string filename){
	std::string::size_type idx;
	idx = filename.rfind('.');

	if(idx != std::string::npos){
		return filename.substr(idx + 1);
	}
	else{
		return "";
	}
}

//------------------------------------------------------------------------------------------------------------
string ofFilePath::removeExt(string filename){
	std::string::size_type idx;
	idx = filename.rfind('.');

	if(idx != std::string::npos){
		return filename.substr(0, idx);
	}
	else{
		return filename;
	}
}


//------------------------------------------------------------------------------------------------------------
string ofFilePath::getPathForDirectory(string path){
	// if a trailing slash is missing from a path, this will clean it up
	// if it's a windows-style "\" path it will add a "\"
	// if it's a unix-style "/" path it will add a "/"
	// TZLA-619 // return Path::forDirectory(path).toString();
    return "";
}

//------------------------------------------------------------------------------------------------------------
string ofFilePath::removeTrailingSlash(string path){
	if(path.length() > 0 && path[path.length() - 1] == '/'){
		path = path.substr(0, path.length() - 1);
	}
	return path;
}


//------------------------------------------------------------------------------------------------------------
string ofFilePath::getFileName(string filePath, bool bRelativeToData){
	/* TZLA-619 // if(bRelativeToData){
		filePath = ofToDataPath(filePath);
	}

	string fileName;

	Path myPath(filePath);
	try{
		fileName = myPath.getFileName();
	}
	catch(Poco::Exception & except){
		return "";
	}

	return fileName;*/
    return "";
}

//------------------------------------------------------------------------------------------------------------
string ofFilePath::getBaseName(string filePath){
	return removeExt(getFileName(filePath));
}

//------------------------------------------------------------------------------------------------------------
string ofFilePath::getEnclosingDirectory(string filePath, bool bRelativeToData){
	/* TZLA-619 // if(bRelativeToData){
		filePath = ofToDataPath(filePath);
	}

	Path myPath(filePath);

	return myPath.parent().toString();*/
    return false;
}

//------------------------------------------------------------------------------------------------------------
string ofFilePath::getAbsolutePath(string path, bool bRelativeToData){
	/* TZLA-619 // if(bRelativeToData){
		path = ofToDataPath(path);
	}

	Path myPath(path);

	return myPath.makeAbsolute().toString();*/
    return false;
}


//------------------------------------------------------------------------------------------------------------
bool ofFilePath::isAbsolute(string path){
	/* TZLA-619 // Path p(path);
	return p.isAbsolute();*/
    return false;
}

//------------------------------------------------------------------------------------------------------------
string ofFilePath::getCurrentWorkingDirectory(){
	#ifdef TARGET_OSX
		char pathOSX[FILENAME_MAX];
		uint32_t size = sizeof(pathOSX);
		if(_NSGetExecutablePath(pathOSX, &size) != 0){
			ofLogError() << "buffer too small; need size " <<  size;
		}
		string pathOSXStr = string(pathOSX);
		string pathWithoutApp;
		size_t found = pathOSXStr.find_last_of("/");
		pathWithoutApp = pathOSXStr.substr(0, found);
		return pathWithoutApp;
	#else
		// TZLA-619 // return Path::current();
        return "";
	#endif
}

string ofFilePath::join(string path1, string path2){
	return removeTrailingSlash(path1) + addLeadingSlash(path2);
}

string ofFilePath::getCurrentExePath(){
	#if defined(TARGET_LINUX) || defined(TARGET_ANDROID)
		char buff[FILENAME_MAX];
		ssize_t size = readlink("/proc/self/exe", buff, sizeof(buff) - 1);
		if (size == -1){
			ofLogError("ofFilePath") << "readlink failed with error " << errno;
		}
		else{
			buff[size] = '\0';
			return buff;
		}
	#elif defined(TARGET_OSX)
		char path[FILENAME_MAX];
		uint32_t size = sizeof(path);
		if(_NSGetExecutablePath(path, &size) == 0){
			ofLogError() << "buffer too small; need size " <<  size;
		}
		return path;
	#elif defined(TARGET_WIN32)
		ofLogError() << "getCurrentExePath() not implemented";
	#endif
	return "";
}

string ofFilePath::getCurrentExeDir(){
	return getEnclosingDirectory(getCurrentExePath(), false);
}

string ofFilePath::getUserHomeDir(){
	#ifndef TARGET_WIN32
			struct passwd * pw = getpwuid(getuid());
		return pw->pw_dir;
	#else
		// getenv will return any Environent Variable on Windows
		// USERPROFILE is the key on Windows 7 but it might be HOME
		// in other flavours of windows...need to check XP and NT...
		return string(getenv("USERPROFILE"));
	#endif
}
