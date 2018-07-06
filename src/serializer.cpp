#include "serializer.h"

#include <QLoggingCategory>
#include <QRegularExpression>
#include <QStringList>

Q_LOGGING_CATEGORY(SerializerLog, "Serializer")

QString Serializer::serialize(const IBoolMatrix &input)
{
    QString output;

    int rowNumberFieldWidth = QString::number(input.rows()).length();

    for (int row = 0; row < input.rows(); ++row)
    {
        auto&& line = QString{ "%1:" }.arg(row, rowNumberFieldWidth);

        for (int col = 0; col < input.columns(); ++col)
        {
            line.append(input.valueAt(col, row) ? "1" : "0");
        }

        output.append(line);
        output.append("\n");
    }

    return output;
}

bool Serializer::deserialize(const QString &input, IBoolMatrix &output)
{
    auto&& lines = input.split(QRegularExpression{ "\r?\n" }, QString::SkipEmptyParts);

    if (lines.count() == 0)
    {
        qCWarning(SerializerLog) << "No lines found in the input";
        return false;
    }

    qCDebug(SerializerLog) << "Line count:" << lines.count();

    for (auto&& line : lines)
    {
        auto&& tokens = line.split(':', QString::SkipEmptyParts);

        if (tokens.count() != 2)
        {
            qCWarning(SerializerLog) << "Skipping invalid input line:" << line;
            continue;
        }

        bool ok;
        int row = tokens[0].trimmed().toInt(&ok);

        if (!ok || row >= output.rows())
        {
            qCWarning(SerializerLog) << "Invalid line number in line:" << line;
            continue;
        }

        auto&& rawData = tokens[1].trimmed();
        qCDebug(SerializerLog) << "Data:" << rawData;

        qCDebug(SerializerLog) << "Loading row:" << row;

        int col = 0;
        for (auto&& value : rawData)
        {
            if (col >= output.columns())
            {
                qCWarning(SerializerLog) << "Skipping extra data after column" << col - 1;
                break;
            }

            output.setValueAt(col, row, value == '1' ? true : false);

            ++col;
        }
    }

    return true;
}
