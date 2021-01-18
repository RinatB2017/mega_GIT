#include <QCoreApplication>
//--------------------------------------------------------------------------------
QString ico_file = "ico/computer.ico";
QString company_name = "Home";
QString file_description = "description";
QString file_version = "0.0.0.0\0";
QString legal_copyright = "Copyright (c) 2021";
QString original_filename = "filename";
QString product_name = "my_product";
QString product_version = "0.0.0.0";
//--------------------------------------------------------------------------------
QString get_result_rc(void)
{
    QString result;

    result.append(QString("#include <windows.h>\n"));
    result.append(QString("\n"));
    result.append(QString("IDI_ICON1	ICON	DISCARDABLE	\"%1\"\n").arg(ico_file));
    result.append(QString("\n"));
    result.append(QString("VS_VERSION_INFO VERSIONINFO\n"));
    result.append(QString("	FILEVERSION %1\n").arg(file_version));
    result.append(QString("	PRODUCTVERSION %1\n").arg(product_version));
    result.append(QString("	FILEFLAGSMASK 0x3fL\n"));
    result.append(QString("#ifdef _DEBUG\n"));
    result.append(QString("	FILEFLAGS VS_FF_DEBUG\n"));
    result.append(QString("#else\n"));
    result.append(QString("	FILEFLAGS 0x0L\n"));
    result.append(QString("#endif\n"));
    result.append(QString("	FILEOS VOS__WINDOWS32\n"));
    result.append(QString("	FILETYPE VFT_DLL\n"));
    result.append(QString("	FILESUBTYPE 0x0L\n"));
    result.append(QString("	BEGIN\n"));
    result.append(QString("		BLOCK \"StringFileInfo\"\n"));
    result.append(QString("		BEGIN\n"));
    result.append(QString("			BLOCK \"040904b0\"\n"));
    result.append(QString("			BEGIN\n"));
    result.append(QString("				VALUE \"CompanyName\", \"%1\"\n").arg(company_name));
    result.append(QString("				VALUE \"FileDescription\", \"%1\"\n").arg(file_description));
    result.append(QString("				VALUE \"FileVersion\", \"%1\"\n").arg(file_version));
    result.append(QString("				VALUE \"LegalCopyright\", \"%1\"\n").arg(legal_copyright));
    result.append(QString("				VALUE \"OriginalFilename\", \"%1\"\n").arg(original_filename));
    result.append(QString("				VALUE \"ProductName\", \"%1\"\n").arg(product_name));
    result.append(QString("				VALUE \"ProductVersion\", \"%1\"\n").arg(product_version));
    result.append(QString("			END\n"));
    result.append(QString("		END\n"));
    result.append(QString("		BLOCK \"VarFileInfo\"\n"));
    result.append(QString("		BEGIN\n"));
    result.append(QString("			VALUE \"Translation\", 0x0409, 1200\n"));
    result.append(QString("		END\n"));
    result.append(QString("	END\n"));
    result.append(QString("/* End of Version info */\n"));

    return result;
}
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return a.exec();
}
