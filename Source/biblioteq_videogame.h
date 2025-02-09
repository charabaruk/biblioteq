/*
** Copyright (c) 2006 - present, Alexis Megas.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. The name of the author may not be used to endorse or promote products
**    derived from BiblioteQ without specific prior written permission.
**
** BIBLIOTEQ IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
** IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
** NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
** BIBLIOTEQ, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _BIBLIOTEQ_VIDEOGAME_H_
#define _BIBLIOTEQ_VIDEOGAME_H_

#include "biblioteq_item.h"
#include "ui_biblioteq_videogameinfo.h"

class biblioteq_videogame: public QMainWindow, public biblioteq_item
{
  Q_OBJECT

 public:
  biblioteq_videogame(biblioteq *parentArg,
		      const QString &oidArg,
		      const QModelIndex &index);
  ~biblioteq_videogame();

  QString fancyTitleForTab(void) const
  {
    auto const title(vg.title->text().trimmed());

    if(title.isEmpty())
      return windowTitle();
    else
      return QString(tr("Video Game (%1)").arg(title));
  }

  void duplicate(const QString &p_oid, const int state);
  void insert(void);
  void modify(const int state);
  void search(const QString &field = "", const QString &value = "");

  void setPublicationDateFormat(const QString &dateFormat)
  {
    vg.release_date->setDisplayFormat(dateFormat);
  }

  void updateWindow(const int state);

 private:
  QString m_engWindowTitle;
  Ui_vgDialog vg;
  void changeEvent(QEvent *event);
  void closeEvent(QCloseEvent *event);
  void prepareFavorites(void);

 private slots:
  void setGlobalFonts(const QFont &font);
  void slotCancel(void);
  void slotDatabaseEnumerationsCommitted(void);
  void slotGo(void);
  void slotPopulateCopiesEditor(void);
  void slotPrepareIcons(void);
  void slotPrint(void);
  void slotPublicationDateEnabled(bool state);
  void slotQuery(void);
  void slotReset(void);
  void slotSelectImage(void);
  void slotShowUsers(void);

 signals:
  void imageChanged(const QImage &image);
  void windowTitleChanged(const QString &text);
};

#endif
