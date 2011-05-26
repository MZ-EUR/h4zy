class Ad
  include DataMapper::Resource
  require 'json'

  belongs_to :campaign
  belongs_to :user
  belongs_to :adgroup
  belongs_to :adtype

  has n, :clicks
  has n, :impressions

  property :id,         Serial
  property :adgroup_id, Integer
  property :adtype_id,  Integer
  property :campaign_id,Integer
  property :user_id,    Integer
  property :pic,        String
  property :content,    Text, :length => 135  # facebook dimensions
  property :title,      String, :length => 25
  property :link,       String, :length => 300 # ?? not sure..
  property :ad_link,    String
  property :created_at, DateTime, :default => lambda { DateTime.now }

  def link=(link)
    attribute_set(:link, link)
    self.ad_link = Digest::SHA1.hexdigest("#{self.link}")
  end

  def trackinglink
    "http://h4zy.info/ads/track?ad=" + self.ad_link
  end

  def img
    # https://s3.amazonaws.com/social_imgs/1-1283528016.jpg
    "https://s3.amazonaws.com/social_imgs/" + self.pic
  end

  def self.social
    Ad.all(:limit => 3, :pic.not => nil)
  end

  def self.socialjson
    ads = Ad.social
    ads.collect do |ad| {:pic => ad.img, :title => ad.title, :link => ad.trackinglink, :content => ad.content} end.to_json
  end

  def self.with_most_impressions
    repository(:default).adapter.select("select a.*, count(b.id) as cnt from ads as a, impressions as b where a.id = b.host_id order by cnt desc limit 10")
  end

  def self.with_most_clicks
    repository(:default).adapter.select("select a.*, count(b.id) as cnt from ads as a, clicks as b where a.id = b.host_id order by cnt desc limit 10")
  end

end
