class Host
  include DataMapper::Resource

  belongs_to :affiliate

  has n, :impressions
  has n, :clicks
  has 1, :download

  property :id,         Serial
  property :affiliate_id, Integer
  property :identity, String, :default => lambda { Digest::SHA1.hexdigest("--#{Time.now.to_i}--") }
  property :os_vers,  String
  property :browser,  String
  property :ip_addr, String
  property :city, String
  property :state, String
  property :country, String
  property :unknown, Boolean, :default => false # sets to true if affiliate_id is mising
  property :credited, Boolean, :default => false
  property :credited_at, DateTime
  property :created_at, DateTime, :default => lambda { DateTime.now }

  def self.with_most_impressions
    repository(:default).adapter.select("select a.*, count(b.id) as cnt from hosts as a, impressions as b where a.id = b.host_id order by cnt desc limit 10")
  end

  def self.with_most_clicks
    repository(:default).adapter.select("select a.*, count(b.id) as cnt from hosts as a, clicks as b where a.id = b.host_id order by cnt desc limit 10")
  end

end
