
#include <iostream>
#include <unistd.h> // unsleep
#include <fstream>  // 파일 입출력
#include <string>   // 문자열 사용
#include <tins/tins.h>

using namespace std;
using namespace Tins;

void usage() {
    cout << "syntax : beacon-flood <interface> <ssid-list-file>\n";
    cout << "sample : beacon-flood mon0 ssid-list.txt\n";
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        usage();
        return -1;
    }

    char *wlan = argv[1];
    char *ssidlist = argv[2];
    string line[10];
    int i=0;

    ifstream ssid(ssidlist);  // ifstream 의 생성자 ssid를 통해 파일열기

    if (!ssid.is_open()){
        cout << "file open error";
        return -1;
    }
    for (int i=0;i<10;i++) {
        getline(ssid,line[i]);
    }
    ssid.close();

    while (true) {

        RadioTap tap;  // instance

        Dot11::address_type ap        = "00:11:22:33:44:55"; // 하드웨어 주소 저장
        Dot11::address_type broadcast = "ff:ff:ff:ff:ff:ff";
        Dot11Beacon beacon;

        beacon.addr1(broadcast); // Destination address
        beacon.addr2(ap); // source address
        beacon.addr3(ap); // Bssid

        beacon.ssid(line[i]);
        beacon.ds_parameter_set(10); // chanel = 10
        beacon.supported_rates({ 1.0f, 5.5f, 11.0f });
        beacon.rsn_information(RSNInformation::wpa2_psk()); //encryption
        tap.inner_pdu(beacon);

        PacketSender sender(wlan);  // interface is wlan0
        sender.send(tap);
        usleep(10000);

        if (line[i] == line[9]) {
          i = -1 ;
        }

        i++;
    }
}
