PShape usa;
PShape state;
String [] Obama  = { "HI", "RI", "CT", "MA", "ME", "NH", "VT", "NY", "NJ", 
  "FL", "NC", "OH", "IN", "IA", "CO", "NV", "PA", "DE", "MD", "MI", 
  "WA", "CA", "OR", "IL", "MN", "WI", "DC", "NM", "VA" };

String [] McCain = { "AK", "GA", "AL", "TN", "WV", "KY", "SC", "WY", "MT", 
  "ID", "TX", "AZ", "UT", "ND", "SD", "NE", "MS", "MO", "AR", "OK", 
  "KS", "LA" };

void setup() {
  size(950, 600);
  // The file Blank_US_Map.svg can be found at Wikimedia Commons
  usa = loadShape("http://upload.wikimedia.org/wikipedia/commons/3/32/Blank_US_Map.svg");
  smooth(); // Improves the drawing quality of the SVG
  noLoop();
}

void draw() {
  background(255);
  // Draw the full map
  shape(usa, 0, 0);
  // Blue denotes states won by Obama
  statesColoring(Obama, color(0, 0, 255));
  // Red  denotes states won by McCain
  statesColoring(McCain, color(255, 0, 0));
  // Save the map as image
  saveFrame("map output.png");
}

void statesColoring(String[] states, int c) {
  for (int i = 0; i < states.length; ++i) {
    PShape state = usa.getChild(states[i]);
    // Disable the colors found in the SVG file
    state.disableStyle();
    // Set our own coloring
    fill(c);
    noStroke();
    // Draw a single state
    shape(state, 0, 0);
  }
}
