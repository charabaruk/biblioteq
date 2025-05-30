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

/*
** The sequence table is required for generating unique integers.
** Please read biblioteq_misc_functions::getSqliteUniqueId().
*/

const char *sqlite_create_schema_text = "\
CREATE TABLE book							\
(									\
    accession_number     TEXT,						\
    alternate_id_1       TEXT,						\
    author               TEXT NOT NULL,					\
    back_cover	         BYTEA,						\
    binding_type         VARCHAR(32) NOT NULL,				\
    book_read            INTEGER DEFAULT 0,				\
    callnumber	         VARCHAR(64),					\
    category	         TEXT NOT NULL,					\
    condition            TEXT,						\
    date_of_reform       VARCHAR(32),					\
    description	         TEXT NOT NULL,					\
    deweynumber	         VARCHAR(64),					\
    edition	         VARCHAR(8) NOT NULL,				\
    front_cover	         BYTEA,						\
    id		         VARCHAR(32) UNIQUE,				\
    isbn13	         VARCHAR(32) UNIQUE,				\
    keyword              TEXT,						\
    language	         VARCHAR(64) NOT NULL DEFAULT 'UNKNOWN',	\
    lccontrolnumber	 VARCHAR(64),					\
    location	         TEXT NOT NULL,					\
    marc_tags            TEXT,						\
    monetary_units	 VARCHAR(64) NOT NULL DEFAULT 'UNKNOWN',	\
    multivolume_set_isbn VARCHAR(32),					\
    myoid	         BIGINT NOT NULL,				\
    origin               TEXT,						\
    originality          TEXT,						\
    pdate	         VARCHAR(32) NOT NULL,				\
    place	         TEXT NOT NULL,					\
    price	         NUMERIC(10, 2) NOT NULL DEFAULT 0.00,		\
    publisher	         TEXT NOT NULL,					\
    purchase_date        VARCHAR(32),					\
    quantity	         INTEGER NOT NULL DEFAULT 1,			\
    series_title         TEXT,						\
    target_audience      TEXT,						\
    title	         TEXT NOT NULL,					\
    type	         VARCHAR(16) NOT NULL DEFAULT 'Book',		\
    url                  TEXT,						\
    volume_number        TEXT						\
);									\
									\
CREATE TABLE book_copy_info						\
(									\
    condition   TEXT,							\
    copy_number	INTEGER NOT NULL DEFAULT 1,				\
    copyid	VARCHAR(64) NOT NULL,					\
    item_oid	BIGINT NOT NULL,					\
    myoid	BIGINT NOT NULL,					\
    notes       TEXT,							\
    originality TEXT,							\
    status      TEXT,							\
    FOREIGN KEY(item_oid) REFERENCES book(myoid) ON DELETE CASCADE,	\
    PRIMARY KEY(copyid, item_oid)					\
);									\
									\
CREATE TABLE book_files							\
(									\
    description	TEXT,							\
    file	BYTEA NOT NULL,						\
    file_digest	TEXT NOT NULL,						\
    file_name   TEXT NOT NULL,						\
    item_oid	BIGINT NOT NULL,					\
    myoid	BIGINT NOT NULL,					\
    FOREIGN KEY(item_oid) REFERENCES book(myoid) ON DELETE CASCADE,	\
    PRIMARY KEY(file_digest, item_oid)					\
);									\
									\
CREATE TABLE book_sequence						\
(									\
    value INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT			\
);									\
									\
CREATE TABLE cd								\
(									\
    accession_number TEXT,						\
    artist	     TEXT NOT NULL,					\
    back_cover	     BYTEA,						\
    category	     TEXT NOT NULL,					\
    cdaudio	     VARCHAR(32) NOT NULL DEFAULT 'Mono',		\
    cddiskcount	     INTEGER NOT NULL DEFAULT 1,			\
    cdformat	     VARCHAR(128) NOT NULL,				\
    cdrecording	     VARCHAR(32) NOT NULL DEFAULT 'Live',		\
    cdruntime	     VARCHAR(32) NOT NULL,				\
    description	     TEXT NOT NULL,					\
    front_cover	     BYTEA,						\
    id		     VARCHAR(32) NOT NULL PRIMARY KEY,			\
    keyword          TEXT,						\
    language	     VARCHAR(64) NOT NULL DEFAULT 'UNKNOWN',		\
    location	     TEXT NOT NULL,					\
    monetary_units   VARCHAR(64) NOT NULL DEFAULT 'UNKNOWN',		\
    myoid	     BIGINT NOT NULL,					\
    price	     NUMERIC(10, 2) NOT NULL DEFAULT 0.00,		\
    quantity	     INTEGER NOT NULL DEFAULT 1,			\
    rdate	     VARCHAR(32) NOT NULL,				\
    recording_label  TEXT NOT NULL,					\
    title	     TEXT NOT NULL,					\
    type	     VARCHAR(16) NOT NULL DEFAULT 'CD'			\
);									\
									\
CREATE TABLE cd_copy_info						\
(									\
    copy_number	INTEGER NOT NULL DEFAULT 1,				\
    copyid	VARCHAR(64) NOT NULL,					\
    item_oid	BIGINT NOT NULL,					\
    myoid	BIGINT NOT NULL,					\
    notes       TEXT,							\
    status	TEXT,							\
    FOREIGN KEY(item_oid) REFERENCES cd(myoid) ON DELETE CASCADE,	\
    PRIMARY KEY(copyid, item_oid)					\
);									\
									\
CREATE TABLE cd_songs							\
(									\
    albumnum  INTEGER NOT NULL DEFAULT 1,				\
    artist    TEXT NOT NULL DEFAULT 'UNKNOWN',				\
    composer  TEXT NOT NULL DEFAULT 'UNKNOWN',				\
    item_oid  BIGINT NOT NULL,						\
    runtime   VARCHAR(32) NOT NULL,					\
    songnum   INTEGER NOT NULL DEFAULT 1,				\
    songtitle VARCHAR(256) NOT NULL,					\
    FOREIGN KEY(item_oid) REFERENCES cd(myoid) ON DELETE CASCADE,	\
    PRIMARY KEY(albumnum, item_oid, songnum)				\
);									\
									\
CREATE TABLE dvd							\
(									\
    accession_number TEXT,						\
    back_cover	     BYTEA,						\
    category	     TEXT NOT NULL,					\
    description	     TEXT NOT NULL,					\
    dvdactor	     TEXT NOT NULL,					\
    dvdaspectratio   VARCHAR(64) NOT NULL,				\
    dvddirector	     TEXT NOT NULL,					\
    dvddiskcount     INTEGER NOT NULL DEFAULT 1,			\
    dvdformat	     TEXT NOT NULL,					\
    dvdrating	     VARCHAR(64) NOT NULL,				\
    dvdregion	     VARCHAR(64) NOT NULL,				\
    dvdruntime	     VARCHAR(32) NOT NULL,				\
    front_cover	     BYTEA,						\
    id		     VARCHAR(32) NOT NULL PRIMARY KEY,			\
    keyword          TEXT,						\
    language	     VARCHAR(64) NOT NULL DEFAULT 'UNKNOWN',		\
    location	     TEXT NOT NULL,					\
    monetary_units   VARCHAR(64) NOT NULL DEFAULT 'UNKNOWN',		\
    myoid	     BIGINT NOT NULL,					\
    price	     NUMERIC(10, 2) NOT NULL DEFAULT 0.00,		\
    quantity	     INTEGER NOT NULL DEFAULT 1,			\
    rdate	     VARCHAR(32) NOT NULL,				\
    studio	     TEXT NOT NULL,					\
    title	     TEXT NOT NULL,					\
    type	     VARCHAR(16) NOT NULL DEFAULT 'DVD'			\
);									\
									\
CREATE TABLE dvd_copy_info						\
(									\
    copy_number	INTEGER NOT NULL DEFAULT 1,				\
    copyid	VARCHAR(64) NOT NULL,					\
    item_oid	BIGINT NOT NULL,					\
    myoid	BIGINT NOT NULL,					\
    notes       TEXT,							\
    status      TEXT,							\
    FOREIGN KEY(item_oid) REFERENCES dvd(myoid) ON DELETE CASCADE,	\
    PRIMARY KEY(copyid, item_oid)					\
);									\
									\
CREATE TABLE grey_literature						\
(									\
    author	    TEXT NOT NULL,					\
    client	    TEXT,						\
    document_code_a TEXT NOT NULL,					\
    document_code_b TEXT NOT NULL,					\
    document_date   TEXT NOT NULL,					\
    document_id	    TEXT NOT NULL PRIMARY KEY,				\
    document_status TEXT,						\
    document_title  TEXT NOT NULL,					\
    document_type   TEXT NOT NULL,					\
    front_cover     BYTEA,						\
    job_number	    TEXT NOT NULL,					\
    location	    TEXT,						\
    myoid	    BIGINT UNIQUE,					\
    notes	    TEXT,						\
    quantity        INTEGER NOT NULL DEFAULT 1,				\
    type	    VARCHAR(16) NOT NULL DEFAULT 'Grey Literature'	\
);									\
									\
CREATE TABLE grey_literature_files					\
(									\
    description	TEXT,							\
    file	BYTEA NOT NULL,						\
    file_digest	TEXT NOT NULL,						\
    file_name   TEXT NOT NULL,						\
    item_oid	BIGINT NOT NULL,					\
    myoid	BIGINT NOT NULL,					\
    FOREIGN KEY(item_oid) REFERENCES grey_literature(myoid) ON		\
                          DELETE CASCADE,				\
    PRIMARY KEY(file_digest, item_oid)					\
);									\
									\
CREATE TABLE journal							\
(									\
    accession_number TEXT,						\
    back_cover	     BYTEA,						\
    callnumber	     VARCHAR(64),					\
    category	     TEXT NOT NULL,					\
    description	     TEXT NOT NULL,					\
    deweynumber	     VARCHAR(64),					\
    front_cover	     BYTEA,						\
    id		     VARCHAR(32),					\
    issueno	     INTEGER NOT NULL DEFAULT 0,			\
    issuevolume	     INTEGER NOT NULL DEFAULT 0,			\
    keyword          TEXT,						\
    language	     VARCHAR(64) NOT NULL DEFAULT 'UNKNOWN',		\
    lccontrolnumber  VARCHAR(64),					\
    location	     TEXT NOT NULL,					\
    marc_tags        TEXT,						\
    monetary_units   VARCHAR(64) NOT NULL DEFAULT 'UNKNOWN',		\
    myoid	     BIGINT NOT NULL,					\
    pdate	     VARCHAR(32) NOT NULL,				\
    place	     TEXT NOT NULL,					\
    price	     NUMERIC(10, 2) NOT NULL DEFAULT 0.00,		\
    publisher	     TEXT NOT NULL,					\
    quantity	     INTEGER NOT NULL DEFAULT 1,			\
    title	     TEXT NOT NULL,					\
    type	     VARCHAR(16) NOT NULL DEFAULT 'Journal',		\
    UNIQUE(id, issueno, issuevolume)					\
);									\
									\
CREATE TABLE journal_copy_info						\
(									\
    copy_number	INTEGER NOT NULL DEFAULT 1,				\
    copyid	VARCHAR(64) NOT NULL,					\
    item_oid	BIGINT NOT NULL,					\
    myoid	BIGINT NOT NULL,					\
    notes       TEXT,							\
    status      TEXT,							\
    FOREIGN KEY(item_oid) REFERENCES journal(myoid) ON DELETE CASCADE,	\
    PRIMARY KEY(copyid, item_oid)					\
);									\
									\
CREATE TABLE journal_files						\
(									\
    description	TEXT,							\
    file	BYTEA NOT NULL,						\
    file_digest	TEXT NOT NULL,						\
    file_name   TEXT NOT NULL,						\
    item_oid	BIGINT NOT NULL,					\
    myoid	BIGINT NOT NULL,					\
    FOREIGN KEY(item_oid) REFERENCES journal(myoid) ON DELETE CASCADE,	\
    PRIMARY KEY(file_digest, item_oid)					\
);									\
									\
CREATE TABLE magazine							\
(									\
    accession_number TEXT,						\
    back_cover	     BYTEA,						\
    callnumber	     VARCHAR(64),					\
    category	     TEXT NOT NULL,					\
    description	     TEXT NOT NULL,					\
    deweynumber	     VARCHAR(64),					\
    front_cover	     BYTEA,						\
    id		     VARCHAR(32),					\
    issueno	     INTEGER NOT NULL DEFAULT 0,			\
    issuevolume	     INTEGER NOT NULL DEFAULT 0,			\
    keyword          TEXT,						\
    language	     VARCHAR(64) NOT NULL DEFAULT 'UNKNOWN',		\
    lccontrolnumber  VARCHAR(64),					\
    location	     TEXT NOT NULL,					\
    marc_tags        TEXT,						\
    monetary_units   VARCHAR(64) NOT NULL DEFAULT 'UNKNOWN',		\
    myoid	     BIGINT NOT NULL,					\
    pdate	     VARCHAR(32) NOT NULL,				\
    place	     TEXT NOT NULL,					\
    price	     NUMERIC(10, 2) NOT NULL DEFAULT 0.00,		\
    publisher	     TEXT NOT NULL,					\
    quantity	     INTEGER NOT NULL DEFAULT 1,			\
    title	     TEXT NOT NULL,					\
    type	     VARCHAR(16) NOT NULL DEFAULT 'Magazine',		\
    UNIQUE(id, issuevolume, issueno)					\
);									\
									\
CREATE TABLE magazine_copy_info						\
(									\
    copy_number	INTEGER NOT NULL DEFAULT 1,				\
    copyid	VARCHAR(64) NOT NULL,					\
    item_oid	BIGINT NOT NULL,					\
    myoid	BIGINT NOT NULL,					\
    notes       TEXT,							\
    status      TEXT,							\
    FOREIGN KEY(item_oid) REFERENCES magazine(myoid) ON DELETE CASCADE,	\
    PRIMARY KEY(copyid, item_oid)				        \
);									\
									\
CREATE TABLE magazine_files						\
(									\
    description	TEXT,							\
    file	BYTEA NOT NULL,						\
    file_digest	TEXT NOT NULL,						\
    file_name   TEXT NOT NULL,						\
    item_oid	BIGINT NOT NULL,					\
    myoid	BIGINT NOT NULL,					\
    FOREIGN KEY(item_oid) REFERENCES magazine(myoid) ON DELETE CASCADE,	\
    PRIMARY KEY(file_digest, item_oid)					\
);									\
									\
CREATE TABLE photograph_collection					\
(									\
    about	     TEXT,						\
    accession_number TEXT,						\
    id		     TEXT PRIMARY KEY NOT NULL,				\
    image	     BYTEA,						\
    image_scaled     BYTEA,						\
    location         TEXT NOT NULL,					\
    myoid	     BIGINT NOT NULL,					\
    notes	     TEXT,						\
    title	     TEXT NOT NULL,					\
    type	     VARCHAR(32) NOT NULL				\
                     DEFAULT 'Photograph Collection'			\
);									\
									\
CREATE TABLE photograph							\
(									\
    accession_number    TEXT,						\
    callnumber		VARCHAR(64),					\
    collection_oid	BIGINT NOT NULL,				\
    copyright		TEXT NOT NULL,					\
    creators		TEXT NOT NULL,					\
    format		TEXT,						\
    id                  TEXT NOT NULL,					\
    image		BYTEA,						\
    image_scaled	BYTEA,						\
    medium		TEXT NOT NULL,					\
    myoid		BIGINT NOT NULL,				\
    notes		TEXT,						\
    other_number	TEXT,						\
    pdate		VARCHAR(32) NOT NULL,				\
    quantity		INTEGER NOT NULL DEFAULT 1,			\
    reproduction_number TEXT NOT NULL,					\
    subjects		TEXT,						\
    title		TEXT NOT NULL,					\
    FOREIGN KEY(collection_oid) REFERENCES				\
                                photograph_collection(myoid) ON		\
				DELETE CASCADE,				\
    PRIMARY KEY(collection_oid, id)					\
);									\
									\
CREATE TABLE videogame							\
(									\
    accession_number TEXT,						\
    back_cover	     BYTEA,						\
    description      TEXT NOT NULL,					\
    developer	     TEXT NOT NULL,					\
    front_cover      BYTEA,						\
    genre	     TEXT NOT NULL,					\
    id		     VARCHAR(32) NOT NULL PRIMARY KEY,			\
    keyword          TEXT,						\
    language	     VARCHAR(64) NOT NULL DEFAULT 'UNKNOWN',		\
    location	     TEXT NOT NULL,					\
    monetary_units   VARCHAR(64) NOT NULL DEFAULT 'UNKNOWN',		\
    myoid	     BIGINT NOT NULL,					\
    place	     TEXT NOT NULL,					\
    price	     NUMERIC(10, 2) NOT NULL DEFAULT 0.00,		\
    publisher	     TEXT NOT NULL,					\
    quantity	     INTEGER NOT NULL DEFAULT 1,			\
    rdate	     VARCHAR(32) NOT NULL,				\
    title	     TEXT NOT NULL,					\
    type	     VARCHAR(16) NOT NULL DEFAULT 'Video Game',		\
    vgmode	     VARCHAR(16) NOT NULL DEFAULT 'Multiplayer',	\
    vgplatform	     VARCHAR(64) NOT NULL,				\
    vgrating	     VARCHAR(64) NOT NULL				\
);									\
									\
CREATE TABLE videogame_copy_info					\
(									\
    copy_number INTEGER NOT NULL DEFAULT 1,				\
    copyid	VARCHAR(64) NOT NULL,					\
    item_oid	BIGINT NOT NULL,					\
    myoid	BIGINT NOT NULL,					\
    notes       TEXT,							\
    status      TEXT,							\
    FOREIGN KEY(item_oid) REFERENCES videogame(myoid) ON		\
                          DELETE CASCADE,				\
    PRIMARY KEY(copyid, item_oid)				        \
);									\
									\
CREATE TRIGGER book_purge_trigger AFTER DELETE ON book			\
FOR EACH row								\
BEGIN									\
    DELETE FROM book_copy_info WHERE item_oid = old.myoid;		\
    DELETE FROM item_borrower WHERE item_oid = old.myoid;		\
    DELETE FROM member_history WHERE item_oid = old.myoid AND		\
                type = old.type;					\
END;									\
									\
CREATE TRIGGER cd_purge_trigger AFTER DELETE ON cd			\
FOR EACH row								\
BEGIN									\
    DELETE FROM cd_copy_info WHERE item_oid = old.myoid;		\
    DELETE FROM cd_songs WHERE item_oid = old.myoid;			\
    DELETE FROM item_borrower WHERE item_oid = old.myoid;		\
    DELETE FROM member_history WHERE item_oid = old.myoid AND		\
                type = old.type;					\
END;									\
									\
CREATE TRIGGER dvd_purge_trigger AFTER DELETE ON dvd			\
FOR EACH row								\
BEGIN									\
    DELETE FROM dvd_copy_info WHERE item_oid = old.myoid;		\
    DELETE FROM item_borrower WHERE item_oid = old.myoid;		\
    DELETE FROM member_history WHERE item_oid = old.myoid AND		\
	        type = old.type;					\
END;									\
									\
CREATE TRIGGER grey_literature_purge_trigger AFTER DELETE ON		\
    grey_literature							\
FOR EACH row								\
BEGIN									\
    DELETE FROM item_borrower WHERE item_oid = old.myoid;		\
    DELETE FROM member_history WHERE item_oid = old.myoid AND		\
                type = old.type;					\
END;									\
									\
CREATE TRIGGER journal_purge_trigger AFTER DELETE ON journal		\
FOR EACH row								\
BEGIN									\
    DELETE FROM item_borrower WHERE item_oid = old.myoid;		\
    DELETE FROM journal_copy_info WHERE item_oid = old.myoid;		\
    DELETE FROM member_history WHERE item_oid = old.myoid AND		\
                type = old.type;					\
END;									\
									\
CREATE TRIGGER magazine_purge_trigger AFTER DELETE ON magazine		\
FOR EACH row								\
BEGIN									\
    DELETE FROM item_borrower WHERE item_oid = old.myoid;		\
    DELETE FROM magazine_copy_info WHERE item_oid = old.myoid;		\
    DELETE FROM member_history WHERE item_oid = old.myoid AND		\
	        type = old.type;					\
END;									\
									\
CREATE TRIGGER videogame_purge_trigger AFTER DELETE ON videogame	\
FOR EACH row								\
BEGIN									\
    DELETE FROM item_borrower WHERE item_oid = old.myoid;		\
    DELETE FROM member_history WHERE item_oid = old.myoid AND		\
      	        type = old.type;					\
    DELETE FROM videogame_copy_info WHERE item_oid = old.myoid;		\
END;									\
									\
CREATE TABLE item_borrower						\
(									\
    copy_number	  INTEGER NOT NULL DEFAULT 1,				\
    copyid	  VARCHAR(64) NOT NULL,					\
    duedate	  VARCHAR(32) NOT NULL,					\
    item_oid	  BIGINT NOT NULL,					\
    memberid	  VARCHAR(16) NOT NULL,					\
    myoid	  INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,		\
    reserved_by	  VARCHAR(128) NOT NULL,				\
    reserved_date VARCHAR(32) NOT NULL,					\
    type	  VARCHAR(16) NOT NULL,					\
    FOREIGN KEY(memberid) REFERENCES member ON DELETE RESTRICT		\
);									\
									\
CREATE TABLE member							\
(									\
    city	                VARCHAR(256) NOT NULL,			\
    comments                    TEXT,					\
    dob		                VARCHAR(32) NOT NULL,			\
    email	                VARCHAR(128),				\
    expiration_date             VARCHAR(32) NOT NULL,			\
    first_name	                VARCHAR(128) NOT NULL,			\
    general_registration_number TEXT,					\
    last_name	                VARCHAR(128) NOT NULL,			\
    maximum_reserved_books      INTEGER NOT NULL DEFAULT 0,		\
    memberclass                 TEXT,					\
    memberid	                VARCHAR(16) NOT NULL PRIMARY KEY	\
                                DEFAULT 1,				\
    membership_fees             NUMERIC(10, 2) NOT NULL DEFAULT 0.00,	\
    membersince	                VARCHAR(32) NOT NULL,			\
    middle_init	                VARCHAR(1),				\
    overdue_fees                NUMERIC(10, 2) NOT NULL DEFAULT 0.00,	\
    sex		                VARCHAR(32) NOT NULL DEFAULT 'Private',	\
    state_abbr	                VARCHAR(16) NOT NULL DEFAULT 'N/A',	\
    street	                VARCHAR(256) NOT NULL,			\
    telephone_num               VARCHAR(32),				\
    zip		                VARCHAR(16) NOT NULL DEFAULT 'N/A'	\
);									\
									\
CREATE TRIGGER item_borrower_trigger AFTER DELETE ON member		\
FOR EACH row								\
BEGIN									\
    DELETE FROM item_borrower WHERE memberid = old.memberid;		\
END;									\
									\
CREATE TABLE member_history						\
(									\
    copyid	  VARCHAR(64) NOT NULL,					\
    duedate	  VARCHAR(32) NOT NULL,					\
    item_oid	  BIGINT NOT NULL,					\
    memberid	  VARCHAR(16) NOT NULL,					\
    myoid	  INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,		\
    reserved_by	  VARCHAR(128) NOT NULL,				\
    reserved_date VARCHAR(32) NOT NULL,					\
    returned_date VARCHAR(32) NOT NULL,					\
    type	  VARCHAR(16) NOT NULL,					\
    FOREIGN KEY(memberid) REFERENCES member(memberid) ON DELETE CASCADE \
);									\
									\
CREATE TRIGGER member_history_trigger AFTER DELETE ON member		\
FOR EACH row								\
BEGIN									\
    DELETE FROM member_history WHERE memberid = old.memberid;		\
END;									\
									\
CREATE TABLE book_binding_types						\
(									\
    binding_type TEXT NOT NULL PRIMARY KEY				\
);									\
									\
CREATE TABLE book_conditions						\
(									\
    condition TEXT NOT NULL PRIMARY KEY					\
);									\
									\
CREATE TABLE book_originality						\
(									\
    originality TEXT NOT NULL PRIMARY KEY				\
);									\
									\
CREATE TABLE book_target_audiences					\
(									\
    target_audience TEXT NOT NULL PRIMARY KEY				\
);									\
									\
CREATE TABLE cd_formats							\
(									\
    cd_format TEXT NOT NULL PRIMARY KEY					\
);                                                                      \
									\
CREATE TABLE dvd_aspect_ratios						\
(									\
    dvd_aspect_ratio TEXT NOT NULL PRIMARY KEY				\
);                                                                      \
									\
CREATE TABLE dvd_ratings						\
(									\
    dvd_rating TEXT NOT NULL PRIMARY KEY				\
);                                                                      \
									\
CREATE TABLE dvd_regions						\
(									\
    dvd_region TEXT NOT NULL PRIMARY KEY				\
);                                                                      \
									\
CREATE TABLE grey_literature_types					\
(									\
    document_type TEXT NOT NULL PRIMARY KEY				\
);									\
									\
CREATE TABLE languages							\
(									\
    language TEXT NOT NULL PRIMARY KEY					\
);      								\
									\
CREATE TABLE locations				                        \
(									\
    location TEXT NOT NULL,						\
    type     VARCHAR(32) NOT NULL,					\
    PRIMARY KEY(location, type)						\
);      								\
									\
CREATE TABLE minimum_days						\
(									\
    days INTEGER NOT NULL,						\
    type VARCHAR(16) NOT NULL PRIMARY KEY				\
);                                                                      \
									\
CREATE TABLE monetary_units						\
(									\
    monetary_unit TEXT NOT NULL PRIMARY KEY				\
);                                                                      \
									\
CREATE TABLE sequence							\
(									\
    value INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT			\
);									\
									\
CREATE TABLE videogame_platforms					\
(									\
    videogame_platform TEXT NOT NULL PRIMARY KEY			\
);                                                                      \
									\
CREATE TABLE videogame_ratings						\
(									\
    videogame_rating TEXT NOT NULL PRIMARY KEY				\
);                                                                      \
";
