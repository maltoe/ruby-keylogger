require 'rake/extensiontask'
require 'rspec/core/rake_task'

spec = Gem::Specification.load('keylogger.gemspec')
Rake::ExtensionTask.new('keylogger', spec)

RSpec::Core::RakeTask.new('spec')
task :spec => [:clean, :compile]

desc "Run the specs with --fail-fast."
RSpec::Core::RakeTask.new(:specff) do |t|
  t.rspec_opts = ["--fail-fast"]
end

task :specff => [:clean, :compile] 
