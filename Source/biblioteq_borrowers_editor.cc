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

#include "biblioteq.h"
#include "biblioteq_borrowers_editor.h"
#include "biblioteq_misc_functions.h"

#include <QScrollBar>
#include <QSettings>
#include <QtMath>

biblioteq_borrowers_editor::biblioteq_borrowers_editor
(QWidget *parent,
 biblioteq *biblioteq,
 biblioteq_item *bitemArg,
 const int quantityArg,
 const QString &ioidArg,
 const QString &uniqueidArg,
 const QFont &font,
 const QString &itemTypeArg,
 const int stateArg):QDialog(parent)
{
  m_bd.setupUi(this);
  m_bitem = bitemArg;
  m_ioid = ioidArg;
  m_identifier = QString(uniqueidArg).remove('-').trimmed();
  m_itemType = itemTypeArg;
  m_quantity = quantityArg;
  m_state = stateArg;
  qmain = biblioteq;

  /*
  ** Override the state, if necessary.
  */

  if(qmain->getRoles().contains("administrator") ||
     qmain->getRoles().contains("circulation"))
    m_state = biblioteq::EDITABLE;
  else if(qmain->getRoles().contains("librarian"))
    m_state = biblioteq::VIEW_ONLY;

  if(m_state == biblioteq::EDITABLE)
    {
      connect(m_bd.eraseButton,
	      SIGNAL(clicked(void)),
	      this,
	      SLOT(slotEraseBorrower(void)));
      connect(m_bd.saveButton,
	      SIGNAL(clicked(void)),
	      this,
	      SLOT(slotSave(void)));
    }
  else
    {
      m_bd.eraseButton->setVisible(false);
      m_bd.saveButton->setVisible(false);
    }

  connect(m_bd.cancelButton,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slotCloseCurrentBorrowers(void)));
  m_bd.table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  prepareIcons();
  setGlobalFonts(font);
  setWindowModality(Qt::ApplicationModal);

  if(!m_identifier.isEmpty())
    setWindowTitle
      (tr("BiblioteQ: Item Reservation Status (") + m_identifier + tr(")"));
  else
    setWindowTitle(tr("BiblioteQ: Item Reservation Status"));
}

biblioteq_borrowers_editor::~biblioteq_borrowers_editor()
{
}

void biblioteq_borrowers_editor::changeEvent(QEvent *event)
{
  if(event)
    switch(event->type())
      {
      case QEvent::LanguageChange:
	{
	  m_bd.retranslateUi(this);
	  break;
	}
      default:
	break;
      }

  QDialog::changeEvent(event);
}

void biblioteq_borrowers_editor::closeEvent(QCloseEvent *event)
{
  Q_UNUSED(event);
  slotCloseCurrentBorrowers();
}

void biblioteq_borrowers_editor::keyPressEvent(QKeyEvent *event)
{
  if(event && event->key() == Qt::Key_Escape)
    slotCloseCurrentBorrowers();

  QDialog::keyPressEvent(event);
}

void biblioteq_borrowers_editor::prepareIcons(void)
{
  QSettings setting;
  auto const index = setting.value
    ("otheroptions/display_icon_set_index", 0).toInt();

  if(index == 1)
    {
      // System.

      m_bd.cancelButton->setIcon
	(QIcon::fromTheme("window-close", QIcon(":/16x16/cancel.png")));
      m_bd.eraseButton->setIcon
	(QIcon::fromTheme("list-remove", QIcon(":/16x16/eraser.png")));
      m_bd.saveButton->setIcon
	(QIcon::fromTheme("dialog-ok", QIcon(":/16x16/ok.png")));
    }
  else
    {
      // Faenza.

      m_bd.cancelButton->setIcon(QIcon(":/16x16/cancel.png"));
      m_bd.eraseButton->setIcon(QIcon(":/16x16/eraser.png"));
      m_bd.saveButton->setIcon(QIcon(":/16x16/ok.png"));
    }
}

void biblioteq_borrowers_editor::setGlobalFonts(const QFont &font)
{
  setFont(font);

  foreach(auto widget, findChildren<QWidget *> ())
    {
      widget->setFont(font);
      widget->update();
    }

  update();
}

void biblioteq_borrowers_editor::showUsers(void)
{
  QDateEdit *dateEdit = nullptr;
  QSqlQuery query(qmain->getDB());
  QString str = "";
  QStringList list;
  QTableWidgetItem *item = nullptr;
  auto terminate = false;
  int i = 0;
  int j = 0;

  m_bd.table->setColumnCount(0);
  m_bd.table->setCurrentItem(nullptr);
  m_bd.table->setRowCount(0);
  m_columnHeaderIndexes.clear();

  if(m_state == biblioteq::EDITABLE)
    {
      list.append(tr("Copy Number"));
      list.append(tr("Barcode"));
      list.append(tr("Member ID"));
      list.append(tr("First Name"));
      list.append(tr("Last Name"));
      list.append(tr("Reservation Date"));
      list.append(tr("Copy Due Date"));
      list.append(tr("Lender"));
      list.append("MYOID");
      m_columnHeaderIndexes.append("Copy Number");
      m_columnHeaderIndexes.append("Barcode");
      m_columnHeaderIndexes.append("Member ID");
      m_columnHeaderIndexes.append("First Name");
      m_columnHeaderIndexes.append("Last Name");
      m_columnHeaderIndexes.append("Reservation Date");
      m_columnHeaderIndexes.append("Copy Due Date");
      m_columnHeaderIndexes.append("Lender");
      m_columnHeaderIndexes.append("MYOID");
    }
  else
    {
      list.append(tr("Copy Number"));
      list.append(tr("Barcode"));
      list.append(tr("Reservation Date"));
      list.append(tr("Copy Due Date"));
      m_columnHeaderIndexes.append("Copy Number");
      m_columnHeaderIndexes.append("Barcode");
      m_columnHeaderIndexes.append("Reservation Date");
      m_columnHeaderIndexes.append("Copy Due Date");
    }

  m_bd.table->horizontalScrollBar()->setValue(0);
  m_bd.table->scrollToTop();
  m_bd.table->setColumnCount(list.size());
  m_bd.table->setHorizontalHeaderLabels(list);
  m_bd.table->setRowCount(m_quantity);

  /*
  ** Hide the OID column.
  */

  if(m_state == biblioteq::EDITABLE)
    m_bd.table->setColumnHidden(m_bd.table->columnCount() - 1, true);

  if(!isVisible())
    updateGeometry();

  show();
  QApplication::processEvents();

  QProgressDialog progress1(this);
  QProgressDialog progress2(this);

  progress1.setLabelText(tr("Constructing objects..."));
  progress1.setMaximum(m_quantity);
  progress1.setMinimum(0);
  progress1.setMinimumWidth
	(qCeil(biblioteq::PROGRESS_DIALOG_WIDTH_MULTIPLIER *
	       progress1.sizeHint().width()));
  progress1.setModal(true);
  progress1.setWindowTitle(tr("BiblioteQ: Progress Dialog"));
  progress1.show();
  progress1.update();
  QApplication::processEvents();

  for(i = 0; i < m_quantity && !progress1.wasCanceled(); i++)
    {
      for(j = 0; j < m_bd.table->columnCount(); j++)
	if(j == 6 && m_state == biblioteq::EDITABLE)
	  {
	    dateEdit = new QDateEdit();
	    dateEdit->setCalendarPopup(true);
	    dateEdit->setDate
	      (QDate::fromString("01/01/2000",
				 biblioteq::s_databaseDateFormat));
	    dateEdit->setEnabled(false);
	    m_bd.table->setCellWidget(i, j, dateEdit);
	  }
	else
	  {
	    item = new QTableWidgetItem();
	    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	    m_bd.table->setItem(i, j, item);
	  }

      if(i + 1 <= progress1.maximum())
	progress1.setValue(i + 1);

      progress1.repaint();
      QApplication::processEvents();
    }

  progress1.close();
  m_bd.table->setRowCount(i); // Support cancellation.

  if(m_state == biblioteq::EDITABLE)
    {
      query.prepare("SELECT borrowers.copy_number, "
		    "borrowers.copyid, "
		    "member.memberid, "
		    "member.first_name, "
		    "member.last_name, "
		    "borrowers.reserved_date, "
		    "borrowers.duedate, "
		    "borrowers.reserved_by, "
		    "borrowers.myoid "
		    "FROM member member, "
		    "item_borrower borrowers "
		    "WHERE borrowers.type = ? AND "
		    "borrowers.item_oid = ? AND "
		    "borrowers.memberid = member.memberid "
		    "ORDER BY borrowers.copy_number");
      query.bindValue(0, m_itemType);
      query.bindValue(1, m_ioid);
    }
  else
    {
      query.prepare("SELECT borrowers.copy_number, "
		    "borrowers.copyid, "
		    "borrowers.reserved_date, "
		    "borrowers.duedate "
		    "FROM "
		    "item_borrower borrowers "
		    "WHERE borrowers.type = ? AND borrowers.item_oid = ? "
		    "ORDER BY borrowers.copy_number");
      query.bindValue(0, m_itemType);
      query.bindValue(1, m_ioid);
    }

  QApplication::setOverrideCursor(Qt::WaitCursor);

  if(!query.exec())
    qmain->addError(tr("Database Error"),
		    tr("Unable to retrieve borrower data."),
		    query.lastError().text(),
		    __FILE__,
		    __LINE__);

  QApplication::restoreOverrideCursor();
  progress2.setLabelText(tr("Retrieving borrower data..."));
  progress2.setMinimum(0);
  progress2.setMinimumWidth
	(qCeil(biblioteq::PROGRESS_DIALOG_WIDTH_MULTIPLIER *
	       progress2.sizeHint().width()));
  progress2.setModal(true);
  progress2.setWindowTitle(tr("BiblioteQ: Progress Dialog"));

  /*
  ** SQLite does not support query.size().
  */

  if(qmain->getDB().driverName() == "QSQLITE")
    {
      if(query.lastError().isValid())
	progress2.setMaximum(0);
      else
	progress2.setMaximum
	  (biblioteq_misc_functions::sqliteQuerySize(query.lastQuery(),
						     query.boundValues(),
						     qmain->getDB(),
						     __FILE__,
						     __LINE__,
						     qmain));
    }
  else
    progress2.setMaximum(query.size());

  progress2.show();
  progress2.update();
  QApplication::processEvents();
  i = -1;

  QDate date;
  auto tomorrow(QDate::currentDate());

  tomorrow = tomorrow.addDays(1);

  while(i++, !progress2.wasCanceled() && query.next())
    {
      if(query.isValid())
	for(j = 0; j < m_bd.table->columnCount(); j++)
	  {
	    if(j == 0)
	      str = query.value(j).toString().trimmed();
	    else if((j == 2 || j == 3) && m_state != biblioteq::EDITABLE)
	      {
		date = QDate::fromString
		  (query.value(j).toString().trimmed(),
		   biblioteq::s_databaseDateFormat);
		str = date.toString(Qt::ISODate);
	      }
	    else if(j == 5 && m_state == biblioteq::EDITABLE)
	      {
		date = QDate::fromString
		  (query.value(j).toString().trimmed(),
		   biblioteq::s_databaseDateFormat);
		str = date.toString(Qt::ISODate);
	      }
	    else
	      str = query.value(j).toString().trimmed();

	    if(j == 6 && m_state == biblioteq::EDITABLE)
	      {
		auto de = qobject_cast<QDateEdit *>
		  (m_bd.table->cellWidget(i, j));

		if(de)
		  {
		    de->setDate
		      (QDate::fromString(str, biblioteq::s_databaseDateFormat));
		    de->setEnabled(true);
		    de->setMinimumDate(tomorrow);
		  }
	      }
	    else if(m_bd.table->item(i, j) != nullptr)
	      m_bd.table->item(i, j)->setText(str);
	    else
	      terminate = true;
	  }

      if(i + 1 <= progress2.maximum())
	progress2.setValue(i + 1);

      progress2.repaint();
      QApplication::processEvents();

      if(terminate)
	break; // Out of resources?
    }

  progress2.close();

  for(int i = 0; i < m_bd.table->columnCount() - 1; i++)
    m_bd.table->resizeColumnToContents(i);
}

void biblioteq_borrowers_editor::slotCloseCurrentBorrowers(void)
{
  deleteLater();
}

void biblioteq_borrowers_editor::slotEraseBorrower(void)
{
  QSqlQuery query(qmain->getDB());
  QString copyid = "";
  QString memberid = "";
  QString oid = "";
  auto const returnedDate = QDateTime::currentDateTime().toString
    (biblioteq::s_databaseDateFormat);
  auto const row = m_bd.table->currentRow();

  if(row < 0)
    {
      QMessageBox::critical
	(this,
	 tr("BiblioteQ: User Error"),
	 tr("Please select the copy that has been returned."));
      QApplication::processEvents();
      return;
    }

  oid = biblioteq_misc_functions::getColumnString
    (m_bd.table, row, m_columnHeaderIndexes.indexOf("MYOID"));

  if(oid.isEmpty())
    {
      QMessageBox::critical(this,
			    tr("BiblioteQ: User Error"),
			    tr("It appears that the selected "
			       "item has not been reserved."));
      QApplication::processEvents();
      return;
    }

  if(QMessageBox::question(this,
			   tr("BiblioteQ: Question"),
			   tr("Are you sure that the copy has been returned?"),
			   QMessageBox::No | QMessageBox::Yes,
			   QMessageBox::No) == QMessageBox::No)
    {
      QApplication::processEvents();
      return;
    }

  QApplication::processEvents();
  query.prepare
    ("DELETE FROM item_borrower WHERE myoid = ? AND type = ?");
  query.bindValue(0, oid);
  query.bindValue(1, m_itemType);
  QApplication::setOverrideCursor(Qt::WaitCursor);

  if(!query.exec())
    {
      QApplication::restoreOverrideCursor();
      qmain->addError(tr("Database Error"),
		      tr("Unable to modify the reservation status of "
			 "the selected copy."),
		      query.lastError().text(),
		      __FILE__,
		      __LINE__);
      QMessageBox::critical(this,
			    tr("BiblioteQ: Database Error"),
			    tr("Unable to modify the reservation status of "
			       "the selected copy."));
      QApplication::processEvents();
      return;
    }
  else
    {
      /*
      ** Record the return in the history table.
      */

      copyid = biblioteq_misc_functions::getColumnString
	(m_bd.table, row, m_columnHeaderIndexes.indexOf("Barcode"));
      memberid = biblioteq_misc_functions::getColumnString
	(m_bd.table, row, m_columnHeaderIndexes.indexOf("Member ID"));
      query.prepare("UPDATE member_history SET returned_date = ? "
		    "WHERE item_oid = ? AND copyid = ? AND "
		    "memberid = ?");
      query.bindValue(0, returnedDate);
      query.bindValue(1, m_ioid);
      query.bindValue(2, copyid);
      query.bindValue(3, memberid);

      if(!query.exec())
	qmain->addError(tr("Database Error"),
			tr("Unable to modify the returned date of "
			   "the selected copy."),
			query.lastError().text(),
			__FILE__,
			__LINE__);

      QApplication::restoreOverrideCursor();
      qmain->updateMembersBrowser(memberid);
      QApplication::setOverrideCursor(Qt::WaitCursor);

      QString availability = "";
      QString errorstr = "";
      QString member = "";
      QString reserved = "";

      availability = biblioteq_misc_functions::getAvailability
	(m_ioid, qmain->getDB(), m_itemType, errorstr);
      reserved = biblioteq_misc_functions::getTotalReserved
	(qmain->getDB(), m_itemType, m_ioid);
      QApplication::restoreOverrideCursor();

      if(!availability.isEmpty())
	biblioteq_misc_functions::updateColumn
	  (qmain->getUI().table,
	   m_bitem->getRow(),
	   qmain->getUI().table->columnNumber("Availability"),
	   availability);

      if(qmain->availabilityColors())
	{
	  QColor color(Qt::white);

	  if(availability.toInt() > 0)
	    color = qmain->availabilityColor(m_itemType);

	  biblioteq_misc_functions::updateColumnColor
	    (qmain->getUI().table,
	     m_bitem->getRow(),
	     qmain->getUI().table->columnNumber("Availability"),
	     color);
	}

      if(!reserved.isEmpty())
	biblioteq_misc_functions::updateColumn
	  (qmain->getUI().table,
	   m_bitem->getRow(),
	   qmain->getUI().table->columnNumber("Total Reserved"),
	   reserved);

      /*
      ** Update the Reservation History panel, if necessary.
      */

      qmain->updateReservationHistoryBrowser
	(memberid, m_ioid, copyid, m_itemType, returnedDate);
      showUsers();

      /*
      ** Update the main window's summary panel, if necessary.
      */

      if(m_bitem &&
	 m_ioid ==
	 biblioteq_misc_functions::getColumnString(qmain->getUI().table,
						   m_bitem->getRow(),
						   "MYOID") &&
	 m_itemType ==
	 biblioteq_misc_functions::getColumnString(qmain->getUI().table,
						   m_bitem->getRow(),
						   qmain->getUI().table->
						   columnNumber("Type")))
	qmain->slotDisplaySummary();

      if(biblioteq_misc_functions::isRequested(qmain->getDB(),
					       m_ioid,
					       m_itemType,
					       errorstr) ||
	 biblioteq_misc_functions::sqliteReturnReminder(member,
							qmain->getDB(),
							m_identifier,
							m_itemType))
	{
	  if(member.trimmed().isEmpty())
	    QMessageBox::information
	      (this,
	       tr("BiblioteQ: Information"),
	       tr("Please set the item aside as another patron has requested "
		  "it."));
	  else
	    QMessageBox::information
	      (this,
	       tr("BiblioteQ: Information"),
	       tr("Please set the item aside as another patron (%1) "
		  "has requested it.").arg(member));

	  QApplication::processEvents();
	}
    }
}

void biblioteq_borrowers_editor::slotSave(void)
{
  QDateEdit *dueDate = nullptr;
  QProgressDialog progress(this);
  QSqlQuery query(qmain->getDB());
  QString oid = "";
  auto const now = QDate::currentDate();
  auto error = false;
  int i = 0;

  progress.setCancelButton(nullptr);
  progress.setLabelText(tr("Updating the due dates..."));
  progress.setMaximum(m_bd.table->rowCount());
  progress.setMinimum(0);
  progress.setMinimumWidth
    (qCeil(biblioteq::PROGRESS_DIALOG_WIDTH_MULTIPLIER *
	   progress.sizeHint().width()));
  progress.setModal(true);
  progress.setWindowTitle(tr("BiblioteQ: Progress Dialog"));
  progress.show();
  progress.repaint();
  QApplication::processEvents();

  for(i = 0; i < m_bd.table->rowCount(); i++)
    {
      oid = biblioteq_misc_functions::getColumnString
	(m_bd.table, i, m_columnHeaderIndexes.indexOf("MYOID"));

      if(!oid.isEmpty())
	{
	  dueDate = qobject_cast<QDateEdit *> (m_bd.table->cellWidget(i, 6));

	  if(!dueDate || dueDate->date() <= now)
	    error = true;
	  else
	    {
	      query.prepare("UPDATE item_borrower "
			    "SET duedate = ? "
			    "WHERE myoid = ? AND "
			    "type = ?");
	      query.bindValue
		(0, dueDate->date().toString(biblioteq::s_databaseDateFormat));
	      query.bindValue(1, oid);
	      query.bindValue(2, m_itemType);

	      if(!query.exec())
		qmain->addError(tr("Database Error"),
				tr("Unable to update the due date."),
				query.lastError().text(),
				__FILE__,
				__LINE__);
	    }
	}

      if(i + 1 <= progress.maximum())
	progress.setValue(i + 1);

      progress.repaint();
      QApplication::processEvents();
    }

  progress.close();

  if(error)
    QMessageBox::critical(this,
			  tr("BiblioteQ: User Error"),
			  tr("Some or all of the Due Dates were not updated "
			     "because of invalid dates."));

  if(query.lastError().isValid())
    QMessageBox::critical(this,
			  tr("BiblioteQ: Database Error"),
			  tr("Some or all of the Due Dates were not updated "
			     "because of database errors."));

  QApplication::processEvents();
}
