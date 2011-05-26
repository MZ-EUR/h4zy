class Impression
  include DataMapper::Resource

  belongs_to :host
  belongs_to :affiliate
  belongs_to :advertiser
  belongs_to :ad

  property :id,         Serial
  property :ad_id,    Integer
  property :affiliate_id,      Integer
  property :advertiser_id,    Integer
  property :host_id,  Integer
  property :cost, Integer
  property :paid, Boolean, :default => false
  property :created_at, DateTime, :default => lambda { DateTime.now }

  def cost_in_dollars
    actual_cost / 100.to_f
  end

  # since CPM = cost/1k impressions
  # we store cost as actual_cost in cents * 1000
  def actual_cost
    self.cost/1000.to_f
  end

end
