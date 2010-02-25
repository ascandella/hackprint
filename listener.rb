#!/usr/bin/env ruby

require 'rubygems'
require 'yaml'
require 'serialport'


@config = YAML.load_file(File.join(File.dirname(__FILE__), '/hackprint.yml'))
@config = @config[@config["station"]]

@serial = SerialPort.new( @config["port"], "baud" => @config["baud"], "data_bits" => 8 )
@open_time = @config["time"].chr
@password = @config["pass"]

loop do
  File.open(@config["authpipe"], 'r') do |f|
    line = f.readline.chomp
    @serial.write(@open_time) if line == @password
    f.close
  end
end

