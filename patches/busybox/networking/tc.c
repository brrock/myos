  #include <stdio.h>
  #include <string.h>
  #include <linux/rtnetlink.h>
  #include <linux/pkt_sched.h>
  #include <linux/types.h>

  /* Define missing CBQ-related constants if not already defined */
  #ifndef TCA_CBQ_MAX
  #define TCA_CBQ_MAX 8
  #endif

  #ifndef TCA_CBQ_RATE
  #define TCA_CBQ_RATE 1
  #endif

  #ifndef TCA_CBQ_LSSOPT
  #define TCA_CBQ_LSSOPT 2
  #endif

  #ifndef TCA_CBQ_WRROPT
  #define TCA_CBQ_WRROPT 3
  #endif

  #ifndef TCA_CBQ_FOPT
  #define TCA_CBQ_FOPT 4
  #endif

  #ifndef TCA_CBQ_OVL_STRATEGY
  #define TCA_CBQ_OVL_STRATEGY 5
  #endif

  /* Placeholder structures for CBQ options */
  struct tc_cbq_lssopt {
      uint32_t flags;
  };

  struct tc_cbq_wrropt {
      uint32_t priority;
      uint32_t weight;
      uint32_t allot;
      uint32_t cpriority;
  };

  struct tc_cbq_fopt {
      // Placeholder fields
  };

  struct tc_cbq_ovl {
      // Placeholder fields
  };

  static void print_rate(char *buf, size_t size, uint32_t rate)
  {
      snprintf(buf, size, "%u", rate);
  }

  static void parse_rtattr(struct rtattr **tb, int max, void *data, int len)
  {
      struct rtattr *rta;
      int remaining;

      for (remaining = len; RTA_OK(rta, remaining); rta = RTA_NEXT(rta, remaining))
      {
          if (rta->rta_type <= max)
              tb[rta->rta_type] = rta;
      }
  }

  static void cbq_print_opt(struct rtattr *opt)
  {
      struct tc_cbq_lssopt *lss = NULL;
      struct tc_cbq_wrropt *wrr = NULL;
      struct tc_cbq_fopt *fopt = NULL;
      struct tc_cbq_ovl *ovl = NULL;
      char buf[64];

      struct rtattr *tb[TCA_CBQ_MAX + 1];

      memset(tb, 0, sizeof(tb));

      parse_rtattr(tb, TCA_CBQ_MAX, RTA_DATA(opt), RTA_PAYLOAD(opt));

      if (tb[TCA_CBQ_RATE]) {
          print_rate(buf, sizeof(buf), *(uint32_t *)RTA_DATA(tb[TCA_CBQ_RATE]));
          printf(" rate %s ", buf);
      }

      if (tb[TCA_CBQ_LSSOPT]) {
          if (RTA_PAYLOAD(tb[TCA_CBQ_LSSOPT]) < sizeof(*lss))
              return;
          lss = RTA_DATA(tb[TCA_CBQ_LSSOPT]);
          if (lss->flags & 1) // Placeholder for actual flags check
              printf(" LSS flags %u", lss->flags);
      }

      if (tb[TCA_CBQ_WRROPT]) {
          if (RTA_PAYLOAD(tb[TCA_CBQ_WRROPT]) < sizeof(*wrr))
              return;
          wrr = RTA_DATA(tb[TCA_CBQ_WRROPT]);
          if (wrr->priority != 0xFFFF) // Placeholder for actual priority check
              printf(" wrr priority %u", wrr->priority);
          if (wrr->weight != 1)
              printf(" weight %u", wrr->weight);
          if (wrr->allot)
              printf(" allot %u", wrr->allot);
      }

      if (tb[TCA_CBQ_FOPT]) {
          if (RTA_PAYLOAD(tb[TCA_CBQ_FOPT]) < sizeof(*fopt))
              return;
          fopt = RTA_DATA(tb[TCA_CBQ_FOPT]);
          // Placeholder for actual fopt processing
      }

      if (tb[TCA_CBQ_OVL_STRATEGY]) {
          if (RTA_PAYLOAD(tb[TCA_CBQ_OVL_STRATEGY]) < sizeof(*ovl))
              return;
          ovl = RTA_DATA(tb[TCA_CBQ_OVL_STRATEGY]);
          // Placeholder for actual ovl processing
      }
  }