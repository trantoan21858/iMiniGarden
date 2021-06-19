import 'dart:async';

import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:get/get.dart';

Future<void> main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp();
  await login();
  runApp(MyApp());
}

Future login() async {
  try {
    await FirebaseAuth.instance.signInWithEmailAndPassword(
        email: "trantoan21858@gmail.com",
        password: "Khanhly2207"
    );
  } on Exception catch (e) {
    print(e.toString());
  }
}

class MyApp extends StatelessWidget {
  StateController controller = Get.put(StateController());
  @override
  Widget build(BuildContext context) {
    return GetMaterialApp(
      title: 'iMiniFarm',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: Scaffold(
        body: SafeArea(
          child: Container(
            margin: EdgeInsets.all(30),
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Divider(
                  color: Colors.black,
                ),
                Container(
                  margin: EdgeInsets.only(top: 20),
                  child: Row(
                    children: [
                      Container(
                        child: Icon(
                          CupertinoIcons.thermometer,
                          color: Colors.red,
                        ),
                        width: 40,
                        height: 40,
                      ),
                      Container(
                        margin: EdgeInsets.only(left: 10),
                        child: Obx(() => Text(
                          "Nhiệt độ: ${controller.tempurature.value}",
                          style: TextStyle(
                              fontSize: 12
                          ),
                        ),),
                      )
                    ],
                  ),
                ),
                Container(
                  margin: EdgeInsets.only(top: 20),
                  child: Row(
                    children: [
                      Container(
                        child: Icon(
                          CupertinoIcons.drop,
                          color: Colors.blue,
                        ),
                        width: 40,
                        height: 40,
                      ),
                      Container(
                        margin: EdgeInsets.only(left: 10),
                        child: Obx(() => Text(
                          'Độ ẩm: ${controller.humidity}',
                          style: TextStyle(
                              fontSize: 12
                          ),
                        )),
                      )
                    ],
                  ),
                ),
                Container(
                  margin: EdgeInsets.only(top: 20),
                  child: Row(
                    children: [
                      Container(
                        child: Icon(
                          CupertinoIcons.person,
                        ),
                        width: 40,
                        height: 40,
                      ),
                      Container(
                        margin: EdgeInsets.only(left: 10),
                        child: Obx(() => Text(
                          "Cảm thấy như: ${controller.heatIndex.value}",
                          style: TextStyle(
                              fontSize: 12
                          ),
                        )),
                      )
                    ],
                  ),
                ),
                Container(
                  margin: EdgeInsets.only(top: 20),
                  child: Row(
                    children: [
                      Container(
                        child: Icon(
                          CupertinoIcons.calendar,
                        ),
                        width: 40,
                        height: 40,
                      ),
                      Container(
                        margin: EdgeInsets.only(left: 10),
                        child: Obx(() => Text(
                          "Lần đo cuối: ${DateTime.fromMillisecondsSinceEpoch(controller.dhtTimestamp.value).toString()}",
                          style: TextStyle(
                              fontSize: 12
                          ),
                        )),
                      )
                    ],
                  ),
                ),
                Divider(
                  color: Colors.black,
                ),
                Container(
                  margin: EdgeInsets.only(top: 20),
                  child: Row(
                    children: [
                      Container(
                        child: Icon(
                          Icons.waves_sharp,
                          color: Colors.brown,
                        ),
                        width: 40,
                        height: 40,
                      ),
                      Container(
                        margin: EdgeInsets.only(left: 10),
                        child: Obx(() => Text(
                          "Độ ẩm đất: ${controller.soilMoisture}",
                          style: TextStyle(
                              fontSize: 12
                          ),
                        )),
                      ),
                    ],
                  ),
                ),
                Container(
                  margin: EdgeInsets.only(top: 20),
                  child: Row(
                    children: [
                      Container(
                        child: Icon(
                          CupertinoIcons.calendar,
                        ),
                        width: 40,
                        height: 40,
                      ),
                      Container(
                        margin: EdgeInsets.only(left: 10),
                        child: Obx(() => Text(
                          "Lần đo cuối: ${DateTime.fromMillisecondsSinceEpoch(controller.soilTimestamp.value).toString()}",
                          style: TextStyle(
                              fontSize: 12
                          ),
                        )),
                      )
                    ],
                  ),
                ),
                Divider(
                  color: Colors.black,
                ),
                Container(
                  margin: EdgeInsets.only(top: 20),
                  child: Row(
                    children: [
                      Container(
                        child: Icon(
                          CupertinoIcons.tree,
                          color: Colors.green,
                        ),
                        width: 40,
                        height: 40,
                      ),
                      Container(
                        margin: EdgeInsets.only(left: 10),
                        child: Obx(() => Text(
                          "Tên cây trồng: ${controller.plantName.value}",
                          style: TextStyle(
                              fontSize: 12
                          ),
                        )),
                      ),
                    ],
                  ),
                ),
                Container(
                  margin: EdgeInsets.only(top: 20),
                  child: Row(
                    children: [
                      Container(
                        child: Icon(
                          CupertinoIcons.time,
                        ),
                        width: 40,
                        height: 40,
                      ),
                      Container(
                        margin: EdgeInsets.only(left: 10),
                        child: Obx(() => Text(
                          "Tuổi: ${((DateTime.now().millisecondsSinceEpoch - controller.plantTimeStamp.value)/Duration.millisecondsPerDay).toInt()} ngày",
                          style: TextStyle(
                              fontSize: 12
                          ),
                        )),
                      ),
                    ],
                  ),
                ),
                Expanded(child: Container()),
                Divider(
                  color: Colors.black,
                ),
                Container(
                  width: double.infinity,
                  margin: EdgeInsets.only(top: 20),
                  child: ElevatedButton(
                    child: Obx(() => Text(
                        controller.pumpOn.value
                            ? 'Đang tưới...'
                            : 'Tưới'
                    )),
                    onPressed: () {
                      controller.onButtonClick();
                    },
                  ),
                )
              ],
            ),
          ),
        ),
      ),
    );
  }
}

class StateController extends GetxController {
  var pumpOn = true.obs;
  DatabaseReference pumpRef;
  StreamSubscription<Event> _pumpStreamSubscription;

  var tempurature = double.nan.obs;
  DatabaseReference tempRef;
  StreamSubscription<Event> _tempStreamSubscriprion;

  var humidity = (-1).obs;
  DatabaseReference humiRef;
  StreamSubscription<Event> _humiStreamSubscriprion;

  var heatIndex = double.nan.obs;
  DatabaseReference heatRef;
  StreamSubscription<Event> _heatStreamSubscriprion;

  var soilMoisture = (-1).obs;
  DatabaseReference soilRef;
  StreamSubscription<Event> _soilStreamSubscriprion;

  var dhtTimestamp = (-1).obs;
  DatabaseReference dhtTimeRef;
  StreamSubscription<Event> _dhtTimeStreamSubscriprion;

  var soilTimestamp = (-1).obs;
  DatabaseReference soilTimeRef;
  StreamSubscription<Event> _soilTimeStreamSubscriprion;

  var plantName = ''.obs;
  DatabaseReference planNameRef;
  StreamSubscription<Event> _planNameStreamSubscriprion;

  var plantTimeStamp = (-1).obs;
  DatabaseReference planTimeRef;
  StreamSubscription<Event> _planTimeStreamSubscriprion;

  StateController() {
    pumpRef = FirebaseDatabase.instance.reference().child('UsersData/' + FirebaseAuth.instance.currentUser.uid + '/devices/50:02:91:E0:DB:51/pump/on');
    _pumpStreamSubscription = pumpRef.onValue.listen((event) {
      pumpOn.value = event.snapshot.value;
    });

    tempRef = FirebaseDatabase.instance.reference().child('UsersData/' + FirebaseAuth.instance.currentUser.uid + '/devices/50:02:91:E0:DB:51/DHTSensor/temperature');
    _tempStreamSubscriprion = tempRef.onValue.listen((event) {
      tempurature.value = event.snapshot.value;
    });

    humiRef = FirebaseDatabase.instance.reference().child('UsersData/' + FirebaseAuth.instance.currentUser.uid + '/devices/50:02:91:E0:DB:51/DHTSensor/humidity');
    _humiStreamSubscriprion = humiRef.onValue.listen((event) {
      humidity.value = event.snapshot.value;
    });

    heatRef = FirebaseDatabase.instance.reference().child('UsersData/' + FirebaseAuth.instance.currentUser.uid + '/devices/50:02:91:E0:DB:51/DHTSensor/heatIndex');
    _heatStreamSubscriprion = heatRef.onValue.listen((event) {
      heatIndex.value = event.snapshot.value;
    });

    dhtTimeRef = FirebaseDatabase.instance.reference().child('UsersData/' + FirebaseAuth.instance.currentUser.uid + '/devices/50:02:91:E0:DB:51/DHTSensor/timestamp');
    _dhtTimeStreamSubscriprion = dhtTimeRef.onValue.listen((event) {
      dhtTimestamp.value = event.snapshot.value;
    });

    soilRef = FirebaseDatabase.instance.reference().child('UsersData/' + FirebaseAuth.instance.currentUser.uid + '/devices/50:02:91:E0:DB:51/SoilMoistureSensor/value');
    _soilStreamSubscriprion = soilRef.onValue.listen((event) {
      soilMoisture.value = event.snapshot.value;
    });

    soilTimeRef = FirebaseDatabase.instance.reference().child('UsersData/' + FirebaseAuth.instance.currentUser.uid + '/devices/50:02:91:E0:DB:51/SoilMoistureSensor/timestamp');
    _soilTimeStreamSubscriprion = soilTimeRef.onValue.listen((event) {
      soilTimestamp.value = event.snapshot.value;
    });

    planNameRef = FirebaseDatabase.instance.reference().child('UsersData/' + FirebaseAuth.instance.currentUser.uid + '/devices/50:02:91:E0:DB:51/plant/name');
    _planNameStreamSubscriprion = planNameRef.onValue.listen((event) {
      plantName.value = event.snapshot.value;
    });

    planTimeRef = FirebaseDatabase.instance.reference().child('UsersData/' + FirebaseAuth.instance.currentUser.uid + '/devices/50:02:91:E0:DB:51/plant/timestamp');
    _planTimeStreamSubscriprion = planTimeRef.onValue.listen((event) {
      if(event.snapshot.value != null)
        plantTimeStamp.value = event.snapshot.value;
    });
  }

  void onButtonClick() {
    if(!pumpOn.value)
      pumpRef.set(true);
  }

  @override
  void dispose() {
    // TODO: implement dispose
    super.dispose();
    _pumpStreamSubscription.cancel();
    _tempStreamSubscriprion.cancel();
    _humiStreamSubscriprion.cancel();
    _heatStreamSubscriprion.cancel();
    _soilStreamSubscriprion.cancel();
    _dhtTimeStreamSubscriprion.cancel();
    _soilTimeStreamSubscriprion.cancel();
    _planTimeStreamSubscriprion.cancel();
    _planNameStreamSubscriprion.cancel();
  }
}