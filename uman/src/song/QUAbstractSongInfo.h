#ifndef QUABSTRACTSONGINFO_H_
#define QUABSTRACTSONGINFO_H_

class QUAbstractSongInfo {
public:
	virtual QString artist() const = 0;
	virtual QString title() const = 0;
	virtual QString mp3() const = 0;
	virtual QString bpm() const = 0;
	virtual QString gap() const = 0;
	virtual QString video() const = 0;
	virtual QString videogap() const = 0;
	virtual QString cover() const = 0;
	virtual QString background() const = 0;
	virtual QString start() const = 0;
	virtual QString language() const = 0;
	virtual QString relative() const = 0;
	virtual QString edition() const = 0;
	virtual QString genre() const = 0;
	virtual QString year() const = 0;
	virtual QString end() const = 0;
	virtual QString creator() const = 0;
};

#endif /* QUABSTRACTSONGINFO_H_ */
