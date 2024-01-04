

//
//byte convertChar(char target) {
//  byte x = 0;
//  // Check for number
//  if ((target < 58) && (target > 47)) return convertNum(target); // Return number is numeric
//  
//  target = bitClear(target, 5); // Upper case
//  switch (target) {
//    case ('A'):
//      x = B1110111;
//      break;
//    case ('B'):
//      x = B1111100;
//      break;
//    case ('C'):
//      x = B1011000;
//      break;
//    case ('D'):
//      x = B1011110;
//      break;
//    case ('E'):
//      x = B1111001;
//      break;
//    case ('F'):
//      x = B1110001;
//      break;
//    case ('H'):
//      x = B1110101;
//      break;
//    case ('I'):
//      x = B0000100;
//      break;
//    case ('J'):
//      x = B0011110;
//      break;
//    case ('K'):
//      x = B1110110;
//      break;
//    case ('L'):
//      x = B0111000;
//      break;
//    case ('N'):
//      x = B1010100;
//      break;
//    case ('O'):
//      x = B1011100;
//      break;
//    case ('P'):
//      x = B1110011;
//      break;
//    case ('Q'):
//      x = B1100111;
//      break;
//    case ('R'):
//      x = B1010000;
//      break;
//    case ('T'):
//      x = B1000110;
//      break;
//    case ('U'):
//      x = B0011100;
//      break;
//    case ('V'):
//      x = B0011100;
//      break;
//    case ('Y'):
//      x = B1101110;
//      break;
//    case ('X'):
//      x = B1110110;
//      break;
//    default:
//      x = 0; // Blank
//      break;
//  }
//  return x;
//}
//
