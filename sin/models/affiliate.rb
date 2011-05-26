class Affiliate
  include DataMapper::Resource

  belongs_to :user

  has n, :clicks
  has n, :impressions
  has n, :downloads
  has n, :hosts
  has n, :credits

  property :id,         Serial
  property :user_id,    Integer
  property :aff_code,   String
  property :updated_at, DateTime
  property :created_at, DateTime, :default => lambda { DateTime.now }

  def self.with_most_impressions
    repository(:default).adapter.select("select a.*, count(b.id) as cnt from affiliates as a, impressions as b where a.id = b.host_id order by cnt desc limit 10")
  end

  def self.with_most_clicks
    repository(:default).adapter.select("select a.*, count(b.id) as cnt from affiliates as a, clicks as b where a.id = b.host_id order by cnt desc limit 10")
  end

end
