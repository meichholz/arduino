Keys = [ "C", "C#", "D", "Eb", "E", "F", "F#", "G", "G#", "A", "Bb", "B" ]
(0 .. 5).each do |octave|
  (1 .. 12).each do |tone|
    printf "%d, // %s in %d\n",
      (Math.exp(Math.log(2.0)*((tone-4+12*octave)/12.0)) *110.0).floor,
        Keys[tone-1],
        octave
  end

end
