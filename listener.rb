#!/usr/bin/env ruby

require 'rubygems'
require 'yaml'
require 'serialport'
require 'logger'


config = YAML.load_file(File.join(File.dirname(__FILE__), '/hackprint.yml'))
@config = config[config["station"]]
logfile = @config['log_file'] || STDERR
@log = Logger.new(logfile)

begin
  @serial = SerialPort.new( @config["port"], "baud" => @config["baud"], "data_bits" => 8 )
rescue Exception => ex
  @log.error "Could not open serial port (#{@config['port']}). Check settings:"
  @log.error ex.to_s
  Process.exit
end

@open_time = @config["time"].chr
@password = @config["pass"]

loop do
  File.open(@config["authpipe"], 'r') do |f|
    line = f.readline.chomp
    if line == @password
      begin
        @log.info "Unlock"
        @serial.write(@open_time)
      rescue Exception => ex
        @log.error "Caught error trying to unlock door: #{ex.inspect}"
      end
    else
      @log.warn "Received bad auth token: '#{line}'"
    end
  end
end

