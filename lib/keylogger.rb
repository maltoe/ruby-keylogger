require 'keylogger.so'

module Keylogger
  def keydown
    self.listen do |c, v|
      v != 1 || yield(c)
    end
  end

  def keyup
    self.listen do |c, v|
      v != 0 || yield(c)
    end
  end

  #-)

  def self.listen_with_keycodes
    keycodes = {}
    file = File.new("/usr/include/linux/input.h", "r")
    while (line = file.gets)
      unless (m = /#define KEY_(\w*)\s*([0-9a-f]*)/.match(line)).nil?
        begin
          k = m[1].downcase.to_sym
          c = Integer(m[2])
          keycodes[c] = k
        rescue => e
          puts e
        end
      end
    end

    self.listen do |c, v|
      yield keycodes[c], v
    end
  end
end
