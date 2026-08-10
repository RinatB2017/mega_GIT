#include <QtGui>

#include "highlighter.h"

//! [0]
Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bABORT\\b"
            << "\\bACTION\\b"
            << "\\bADD\\b"
            << "\\bAFTER\\b"
            << "\\bALL\\b"
            << "\\bALTER\\b"
            << "\\bANALYZE\\b"
            << "\\bAND\\b"
            << "\\bAS\\b"
            << "\\bASC\\b"
            << "\\bATTACH\\b"
            << "\\bAUTOINCREMENT\\b"
            << "\\bBEFORE\\b"
            << "\\bBEGIN\\b"
            << "\\bBETWEEN\\b"
            << "\\bBY\\b"
            << "\\bCASCADE\\b"
            << "\\bCASE\\b"
            << "\\bCAST\\b"
            << "\\bCHECK\\b"
            << "\\bCOLLATE\\b"
            << "\\bCOLUMN\\b"
            << "\\bCOMMIT\\b"
            << "\\bCONFLICT\\b"
            << "\\bCONSTRAINT\\b"
            << "\\bCREATE\\b"
            << "\\bCROSS\\b"
            << "\\bCURRENT_DATE\\b"
            << "\\bCURRENT_TIME\\b"
            << "\\bCURRENT_TIMESTAMP\\b"
            << "\\bDATABASE\\b"
            << "\\bDEFAULT\\b"
            << "\\bDEFERRABLE\\b"
            << "\\bDEFERRED\\b"
            << "\\bDELETE\\b"
            << "\\bDESC\\b"
            << "\\bDETACH\\b"
            << "\\bDISTINCT\\b"
            << "\\bDROP\\b"
            << "\\bEACH\\b"
            << "\\bELSE\\b"
            << "\\bEND\\b"
            << "\\bESCAPE\\b"
            << "\\bEXCEPT\\b"
            << "\\bEXCLUSIVE\\b"
            << "\\bEXISTS\\b"
            << "\\bEXPLAIN\\b"
            << "\\bFAIL\\b"
            << "\\bFOR\\b"
            << "\\bFOREIGN\\b"
            << "\\bFROM\\b"
            << "\\bFULL\\b"
            << "\\bGLOB\\b"
            << "\\bGROUP\\b"
            << "\\bHAVING\\b"
            << "\\bIF\\b"
            << "\\bIGNORE\\b"
            << "\\bIMMEDIATE\\b"
            << "\\bIN\\b"
            << "\\bINDEX\\b"
            << "\\bINDEXED\\b"
            << "\\bINITIALLY\\b"
            << "\\bINNER\\b"
            << "\\bINSERT\\b"
            << "\\bINSTEAD\\b"
            << "\\bINTERSECT\\b"
            << "\\bINTO\\b"
            << "\\bIS\\b"
            << "\\bISNULL\\b"
            << "\\bJOIN\\b"
            << "\\bKEY\\b"
            << "\\bLEFT\\b"
            << "\\bLIKE\\b"
            << "\\bLIMIT\\b"
            << "\\bMATCH\\b"
            << "\\bNATURAL\\b"
            << "\\bNO\\b"
            << "\\bNOT\\b"
            << "\\bNOTNULL\\b"
            << "\\bNULL\\b"
            << "\\bOF\\b"
            << "\\bOFFSET\\b"
            << "\\bON\\b"
            << "\\bOR\\b"
            << "\\bORDER\\b"
            << "\\bOUTER\\b"
            << "\\bPLAN\\b"
            << "\\bPRAGMA\\b"
            << "\\bPRIMARY\\b"
            << "\\bQUERY\\b"
            << "\\bRAISE\\b"
            << "\\bREFERENCES\\b"
            << "\\bREGEXP\\b"
            << "\\bREINDEX\\b"
            << "\\bRELEASE\\b"
            << "\\bRENAME\\b"
            << "\\bREPLACE\\b"
            << "\\bRESTRICT\\b"
            << "\\bRIGHT\\b"
            << "\\bROLLBACK\\b"
            << "\\bROW\\b"
            << "\\bSAVEPOINT\\b"
            << "\\bSELECT\\b"
            << "\\bSET\\b"
            << "\\bTABLE\\b"
            << "\\bTEMP\\b"
            << "\\bTEMPORARY\\b"
            << "\\bTHEN\\b"
            << "\\bTO\\b"
            << "\\bTRANSACTION\\b"
            << "\\bTRIGGER\\b"
            << "\\bUNION\\b"
            << "\\bUNIQUE\\b"
            << "\\bUPDATE\\b"
            << "\\bUSING\\b"
            << "\\bVACUUM\\b"
            << "\\bVALUES\\b"
            << "\\bVIEW\\b"
            << "\\bVIRTUAL\\b"
            << "\\bWHEN\\b"
            << "\\bWHERE\\b";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
//! [0] //! [1]
    }
//! [1]

//! [2]
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);
//! [2]

//! [3]
    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("--[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);
//! [3]

//! [4]
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("'.*'");
    rule.format = quotationFormat;
    highlightingRules.append(rule);
//! [4]

//! [5]
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);
//! [5]

//! [6]
    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}
//! [6]

//! [7]
void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
//! [7] //! [8]
    setCurrentBlockState(0);
//! [8]

//! [9]
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

//! [9] //! [10]
    while (startIndex >= 0) {
//! [10] //! [11]
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
//! [11]
