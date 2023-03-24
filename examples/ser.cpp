#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>

#include <vsomeip/vsomeip.hpp>


#define SAMPLE_SERVICE_ID       0x1212
#define SAMPLE_INSTANCE_ID      0x1234
#define SAMPLE_METHOD_ID        0x0421

#define SAMPLE_EVENT_ID         0x8777
#define SAMPLE_GET_METHOD_ID    0x0001
#define SAMPLE_SET_METHOD_ID    0x0002

#define SAMPLE_EVENT_ID1         0x8778
#define SAMPLE_GET_METHOD_ID1    0x0001
#define SAMPLE_SET_METHOD_ID1    0x0002

#define SAMPLE_EVENT_ID2         0x8779
#define SAMPLE_GET_METHOD_ID2    0x0001
#define SAMPLE_SET_METHOD_ID2    0x0002

#define SAMPLE_EVENTGROUP_ID    0x4455
#define SAMPLE_EVENTGROUP_ID1    0x4465
#define SAMPLE_EVENTGROUP_ID2    0x4475

#define OTHER_SAMPLE_SERVICE_ID 0x3434
#define OTHER_SAMPLE_INSTANCE_ID 0x3456
#define OTHER_SAMPLE_METHOD_ID  0x1421

#define SAMPLE_SERVICE_ID3       0x1213
#define SAMPLE_INSTANCE_ID3      0x1233
#define SAMPLE_METHOD_ID3        0x0423

#define SAMPLE_EVENT_ID3         0x8773
#define SAMPLE_GET_METHOD_ID3    0x0001
#define SAMPLE_SET_METHOD_ID3    0x0002


#define SAMPLE_EVENTGROUP_ID3    0x4453


std::shared_ptr<vsomeip::application> app;

void on_message(const std::shared_ptr<vsomeip::message> &_request) {

    std::shared_ptr<vsomeip::payload> its_payload = _request->get_payload();
    vsomeip::length_t l = its_payload->get_length();

    // Get payload
    std::stringstream ss;
    for (vsomeip::length_t i=0; i<l; i++) {
       ss << std::setw(2) << std::setfill('0') << std::hex
          << (int)*(its_payload->get_data()+i) << " ";
    }

    std::cout << "SERVICE: Received message with Client/Session ["
        << std::setw(4) << std::setfill('0') << std::hex << _request->get_client() << "/"
        << std::setw(4) << std::setfill('0') << std::hex << _request->get_session() << "] "
        << ss.str() << std::endl;

    // Create response
    std::shared_ptr<vsomeip::message> its_response = vsomeip::runtime::get()->create_response(_request);
    its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    for (int i=9; i>=0; i--) {
        its_payload_data.push_back(i % 256);
    }
    its_payload->set_data(its_payload_data);
    its_response->set_payload(its_payload);
//    app->send(its_response, true);

}
#include <pthread.h>
pthread_t tid;
void* nots(void *p){

    vsomeip::byte_t its_data[] = { 0x10 };

  std::shared_ptr< vsomeip::payload > payload = vsomeip::runtime::get()->create_payload();
  
while(1){
int i=0xffff;
char input = getchar();
if(input == 'a'){
   payload->set_data(its_data, sizeof(its_data));
   app->notify(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENT_ID, payload);
   }
else if(input == 'b'){
   payload->set_data(its_data, sizeof(its_data));
   app->notify(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENT_ID1, payload);
   }   
else if(input == 'c')
{
   payload->set_data(its_data, sizeof(its_data));
   app->notify(SAMPLE_SERVICE_ID3, SAMPLE_INSTANCE_ID3, SAMPLE_EVENT_ID3, payload);
}

while (i--);
its_data[0]+=1;
}



}
int main() {

   app = vsomeip::runtime::get()->create_application("service-sample");
   app->init();
   app->register_message_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_METHOD_ID, on_message);
   app->register_message_handler(OTHER_SAMPLE_SERVICE_ID, OTHER_SAMPLE_INSTANCE_ID, OTHER_SAMPLE_METHOD_ID, on_message);
   app->register_message_handler(SAMPLE_SERVICE_ID3, SAMPLE_INSTANCE_ID3, SAMPLE_METHOD_ID3, on_message);   
   app->offer_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
   app->offer_service(OTHER_SAMPLE_SERVICE_ID, OTHER_SAMPLE_INSTANCE_ID);
   app->offer_service(SAMPLE_SERVICE_ID3, SAMPLE_INSTANCE_ID3);   
   std::set<vsomeip::eventgroup_t> its_groups;
   its_groups.insert(SAMPLE_EVENTGROUP_ID);
   app->offer_event(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENT_ID, its_groups);
   app->offer_event(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENT_ID1, its_groups);
   
   its_groups.insert(SAMPLE_EVENTGROUP_ID3);
   app->offer_event(SAMPLE_SERVICE_ID3, SAMPLE_INSTANCE_ID3, SAMPLE_EVENT_ID3, its_groups);
     
   pthread_create(&tid,NULL,nots,NULL);
   app->start();
}
