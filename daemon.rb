#!/usr/bin/env ruby

require 'rubygems'
require 'daemons'

options = {
   :app_name   => "hackprintd",
   :ARGV       => ['start', '-f'],
   :dir_mode   => :script,
   :dir        => 'pids',
   :multiple   => false,
   :ontop      => false,
   :mode       => :exec,
   :backtrace  => true,
   :monitor    => true
 }

 Daemons.run(File.join(File.dirname(__FILE__), 'listener.rb'), options)
