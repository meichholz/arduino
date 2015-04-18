Keys = [ "C", "C#", "D", "Eb", "E", "F", "F#", "G", "G#", "A", "Bb", "B" ]
(-1 .. 5).each do |octave|
  (0 .. 11).each do |tone|
    printf "%d, // %s in %d\n",
      (Math.exp(Math.log(2.0)*((tone+3+12*octave)/12.0)) *110.0).floor,
        Keys[tone],
        octave
  end

end
