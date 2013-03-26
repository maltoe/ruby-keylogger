Gem::Specification.new do |s|
	s.name        = "keylogger"
	s.version     = "0.0.1"
	s.summary     = "A keylogger for Ruby"
	s.description = "Listens to global key events and signals them to Ruby."
	s.homepage    = "http://www.github.com/maltoe/"
	s.author      = "Malte Rohde"
	s.email	      = "malte.rohde@gmx.de"

	s.files = Dir.glob("lib/**/*.rb")
			  
	s.extensions << "ext/keylogger/extconf.rb"

	s.add_development_dependency "rake-compiler"
end
