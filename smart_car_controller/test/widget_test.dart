import 'package:flutter_test/flutter_test.dart';
import 'package:smart_car_controller/main.dart';

void main() {
  testWidgets('renders the Smart Car controller shell', (tester) async {
    await tester.pumpWidget(const SmartCarApp());
    expect(find.text('Smart Car Controller'), findsOneWidget);
    expect(find.text('CONNECTED'), findsNWidgets(2));
    expect(find.text('EMERGENCY STOP'), findsOneWidget);
  });
}
