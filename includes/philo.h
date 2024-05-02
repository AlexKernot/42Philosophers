/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@42adel.org.au>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 16:25:35 by akernot           #+#    #+#             */
/*   Updated: 2023/08/25 16:37:28 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <stdint.h>

typedef struct timeval	t_timeval;

typedef enum e_state
{
	eating = 0,
	thinking = 1,
	sleeping = 2,
	dead = 3,
	fork_taken = 4,
	finished = 5
}	t_state;

typedef enum e_bool
{
	false = 0,
	true = 1
}	t_bool;

typedef struct s_fork
{
	t_bool			taken;
	pthread_mutex_t	lock;
}	t_fork;

typedef struct s_program_data
{
	t_timeval		time_0;
	pthread_mutex_t	announcing;
	t_bool			philo_died;
	int				philos_fin;
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_times_philo_eat;
}	t_program_data;

typedef struct s_packet
{
	t_program_data	*data;
	t_fork			*right_fork;
	t_fork			*left_fork;
	int				philo_id;
}	t_packet;

typedef struct s_philo
{
	int		philo_id;
	int		state;
	int64_t	last_ate;
	int64_t	time_end_action;
	t_fork	*left_fork;
	t_fork	*right_fork;
}	t_philo;

void	live(t_program_data *data, t_philo *philo);
t_fork	*create_forks(t_program_data *data);
t_bool	take_forks(t_philo *philo);
void	return_forks(t_philo *philo);
void	*init_philo(void *philo_packet);
void	announce(t_program_data *data, int philo_id, t_state state);
int64_t	timestamp(t_timeval *time_0, t_timeval *time);
int		ft_atoi(const char *nptr);
void	wait_out(t_program_data *data, pthread_t *philos);

#endif
