/***********************************************
    Copyright (C) 2014  Schutz Sacha
    This file is part of QJsonModel (https://github.com/dridk/QJsonmodel).

    QJsonModel is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    QJsonModel is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QJsonModel.  If not, see <http://www.gnu.org/licenses/>.

**********************************************/

#include <QApplication>
#include <QTreeView>
#include <QFile>
#include <string>
#include "qjsonmodel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTreeView * view   = new QTreeView;
    QJsonModel * model = new QJsonModel;

    view->setModel(model);

    //    std::string json = R"({
    //                       "firstName": "John",
    //                       "lastName": "Smith",
    //                       "age": 25,
    //                       "address":
    //                       {
    //                           "streetAddress": "21 2nd Street",
    //                           "city": "New York",
    //                           "state": "NY",
    //                           "postalCode": "10021"
    //                       },
    //                       "phoneNumber":
    //                       [
    //                           {
    //                             "type": "home",
    //                             "number": "212 555-1234"
    //                           },
    //                           {
    //                             "type": "fax",
    //                             "number": "646 555-4567"
    //                           }
    //                       ]
    //                   })";

    QString json = "{\"status\":{\"timestamp\":\"2020-07-01T22:57:39.423Z\",\"error_code\":0,\"error_message\":null,\"elapsed\":26,\"credit_count\":1,\"notice\":null},\"data\":{\"BTC\":{\"id\":1,\"name\":\"Bitcoin\",\"symbol\":\"BTC\",\"slug\":\"bitcoin\",\"num_market_pairs\":8306,\"date_added\":\"2013-04-28T00:00:00.000Z\",\"tags\":[\"mineable\",\"state-channels\",\"sha-256\",\"privacy\",\"pow\",\"store-of-value\"],\"max_supply\":21000000,\"circulating_supply\":18420268,\"total_supply\":18420268,\"is_active\":1,\"platform\":null,\"cmc_rank\":1,\"is_fiat\":0,\"last_updated\":\"2020-07-01T22:56:33.000Z\",\"quote\":{\"USD\":{\"price\":9232.22783688,\"volume_24h\":15957216692.0124,\"percent_change_1h\":-0.0745469,\"percent_change_24h\":0.870893,\"percent_change_7d\":-0.65612,\"market_cap\":170060110992.3899,\"last_updated\":\"2020-07-01T22:56:33.000Z\"}}}}}";

    QByteArray ba;
    ba.append(json);
    model->loadJson(ba);
    view->show();

    return a.exec();
}
