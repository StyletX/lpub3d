 
/****************************************************************************
**
** Copyright (C) 2007-2009 Kevin Clague. All rights reserved.
** Copyright (C) 2015 - 2018 Trevor SANDY. All rights reserved.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

/****************************************************************************
 *
 * This class reads in, manages and writes out LDraw files.  While being
 * edited an MPD file, or a top level LDraw files and any sub-model files
 * are maintained in memory using this class.
 *
 * Please see lpub.h for an overall description of how the files in LPub
 * make up the LPub program.
 *
 ***************************************************************************/

#ifndef LDRAWFILES_H
#define LDRAWFILES_H

#include <QStringList>
#include <QString>
#include <QMap>
#include <QDateTime>
#include <QList>

#include "excludedparts.h"
#include "QsLog.h"

extern QList<QRegExp> LDrawHeaderRegExp;

class LDrawSubFile {
  public:
    QStringList _contents;
    bool        _modified;
    QDateTime   _datetime;
    int         _numSteps;
    bool        _beenCounted;
    int         _instances;
    int         _mirrorInstances;
    bool        _rendered;
    bool        _mirrorRendered;
    bool        _changedSinceLastWrite;
    bool        _unofficialPart;
    bool        _generated;
    int         _prevStepPosition;
    int         _startPageNumber;

    LDrawSubFile()
    {
      _unofficialPart = false;
    }
    LDrawSubFile(
            const QStringList &contents,
            QDateTime   &datetime,
            bool         unofficialPart,
            bool         generated = false);
    ~LDrawSubFile()
    {
      _contents.clear();
    }
};

class ViewerStep {
  public:
    QStringList _contents;
    QString   	_filePath;
    bool        _modified;
    bool        _multiStep;
    bool        _calledOut;

    ViewerStep()
    {
      _modified = false;
    }
    ViewerStep(
      const QStringList &contents,
      const QString     &filePath,
      bool               multiStep,
      bool               calledOut);
    ~ViewerStep()
    {
      _contents.clear();
    }
};

class LDrawFile {
  private:
    QMap<QString, LDrawSubFile> _subFiles;
    QMap<QString, ViewerStep>   _viewerSteps;
    QStringList                 _emptyList;
    QString                     _emptyString;
    bool                        _mpd;
    static int                  _emptyInt;

    ExcludedParts               excludedParts; // internal list of part count excluded parts
  public:
    LDrawFile();
    ~LDrawFile()
    {
      _subFiles.empty();
    }

    QStringList                 _subFileOrder;
    static QStringList          _uniqueParts;
    static QStringList          _missingParts;
    static QString              _file;
    static QString              _name;
    static QString              _author;
    static QString              _description;
    static QString              _category;
    static int                  _partCount;

    int getPartCount(){
      return _partCount;
    }

    bool saveFile(const QString &fileName);
    bool saveMPDFile(const QString &filename);
    bool saveLDRFile(const QString &filename);

    void insert(const QString     &fileName,
                      QStringList &contents,
                      QDateTime   &datetime,
                      bool         unofficialPart,
                      bool         generated = false);

    int  size(const QString &fileName);
    void empty();

    QStringList contents(const QString &fileName);
    void setContents(const QString     &fileName, 
                     const QStringList &contents);
    void setPrevStepPosition(const QString &mcFileName,
                         const int     &prevStepPosition);
    int getPrevStepPosition(const QString &mcFileName);
    void clearPrevStepPositions();
    void setModelStartPageNumber(const QString &mcFileName,
                         const int     &startPageNumber);
    int getModelStartPageNumber(const QString &mcFileName);
    void subFileLevels(QStringList &contents, int &level);
    void loadFile(const QString &fileName);
    void loadMPDFile(const QString &fileName, QDateTime &datetime);
    void loadLDRFile(const QString &path, const QString &fileName, bool topLevel = true);
    QStringList subFileOrder();
    
    QString readLine(const QString &fileName, int lineNumber);
    void insertLine( const QString &fileName, int lineNumber, const QString &line);
    void replaceLine(const QString &fileName, int lineNumber, const QString &line);
    void deleteLine( const QString &fileName, int lineNumber);
    void changeContents(const QString &fileName, 
                              int      position, 
                              int      charsRemoved, 
                        const QString &charsAdded);

    bool isMpd();
    QString topLevelFile();
    bool isUnofficialPart(const QString &name);
    int numSteps(const QString &fileName);
    QDateTime lastModified(const QString &fileName);
    bool contains(const QString &file);
    bool isSubmodel(const QString &file);
    bool modified();
    bool modified(const QString &fileName);
    bool older(const QStringList &parsedStack,
               const QDateTime &lastModified);
    static bool mirrored(const QStringList &tokens);
    void unrendered();
    void setRendered(const QString &fileName, bool mirrored);
    bool rendered(const QString &fileName, bool mirrored);
    int instances(const QString &fileName, bool mirrored);
    void countParts(const QString &fileName);
    void countInstances();
    void countInstances(const QString &fileName, bool mirrored, const bool callout = false);
    bool changedSinceLastWrite(const QString &fileName);
    void tempCacheCleared();

    /* ViewerStep functions */
    void insertViewerStep(const QString     &fileName,
                          const QStringList &contents,
                          const QString     &filePath,
                          bool               multiStep,
                          bool               calledOut);
    void updateViewerStep(const QString     &fileName,
                          const QStringList &contents);
    QStringList getViewerStepContents(const QString &fileName);
    QString     getViewerStepFilePath(const QString &fileName);
    bool        isViewerStepMultiStep(const QString &fileName);
    bool        isViewerStepCalledOut(const QString &fileName);
    bool        viewerStepContentExist(      const QString &fileName);
    void        clearViewerSteps();
};

int split(const QString &line, QStringList &argv);
int validSoQ(const QString &line, int soq);
bool isHeader(QString &line);
bool isComment(QString &line);
bool isUnofficialFileType(QString &line);

#endif
